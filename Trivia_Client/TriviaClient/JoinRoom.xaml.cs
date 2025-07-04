using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Threading;

namespace TriviaClient
{
    public partial class JoinRoom : Window, INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        public static string LoggedUsername => Communicator.GetLoggedUsername();

        private List<Room> rooms;
        public List<Room> Rooms
        {
            get => rooms;
            set
            {
                rooms = value;
                OnPropertyChanged();
            }
        }

        private Room selectedRoom;
        public Room SelectedRoom
        {
            get => selectedRoom;
            set
            {
                selectedRoom = value;
                OnPropertyChanged();
            }
        }

        // Here is the thread that refreshes every 3 seconds (to get the new updated rooms list)
        private Thread getRoomsContinuously;
        private bool shouldStop = false; // Flag to control the thread

        public JoinRoom()
        {
            InitializeComponent();
            DataContext = this;

            getRoomsContinuously = new Thread(GetRooms);
            getRoomsContinuously.Start();
        }

        private void GetRooms()
        {
            try
            {
                while (!shouldStop)
                {
                    GetRoomsResponse serverResponse = Communicator.GetRooms();

                    if (serverResponse.status == 0)
                    {
                        Application.Current.Dispatcher.Invoke(() =>
                        {
                            Menu menuWindow = new Menu();
                            menuWindow.Show();
                            this.Close();
                        });
                        return;
                    }

                    Application.Current.Dispatcher.Invoke(() =>
                    {
                        Rooms = serverResponse.rooms;
                    });

                    Thread.Sleep(3000);
                }
            }
            catch (Exception ex)
            {
                // Handle exceptions appropriately
                Console.WriteLine($"An error occurred: {ex.Message}");
            }
        }

        protected override void OnClosed(EventArgs e)
        {
            base.OnClosed(e);

            // Signal the thread to stop
            shouldStop = true;

            // Optionally wait for the thread to finish
            getRoomsContinuously?.Join();
        }

        private void Border_MouseDown(object sender, MouseButtonEventArgs e)
        {
            this.DragMove();
        }

        private void CloseImage_MouseUp(object sender, MouseButtonEventArgs e)
        {
            if (Communicator.GetIsCreated())
                Communicator.Logout();

            Application.Current.Shutdown();
        }

        private void MoveToCreateRoom(object sender, RoutedEventArgs e)
        {
            CreateRoom createRoomWindow = new CreateRoom();
            createRoomWindow.Show();
            this.Close();
        }

        private void MoveToStatistics(object sender, RoutedEventArgs e)
        {
            Statistics statisticsWindow = new Statistics();
            statisticsWindow.Show();
            this.Close();
        }

        private void MoveToHighScores(object sender, RoutedEventArgs e)
        {
            HighScores highScoresWindow = new HighScores();
            highScoresWindow.Show();
            this.Close();
        }

        private void MoveToLogout(object sender, RoutedEventArgs e)
        {
            if (Communicator.Logout())
            {
                // Get the path to the current executable
                string applicationPath = Process.GetCurrentProcess().MainModule.FileName;

                // Start a new instance of the application
                Process.Start(applicationPath);

                // Shutdown the current application
                Application.Current.Shutdown();
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (SelectedRoom == null)
            {
                MessageBox.Show("Please select a room first.");
                return;
            }

            bool joinSuccess = Communicator.JoinRoom(SelectedRoom.id);

            if (!joinSuccess)
            {
                // Server already showed the message (via JoinRoom)
                return;
            }

            // Join was successful
            //MessageBox.Show("Joined the selected room successfully.");
            MemberWaitingRoom waitingRoomWindow = new MemberWaitingRoom(SelectedRoom.name);
            waitingRoomWindow.Show();
            this.Close();
        }

        protected void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}