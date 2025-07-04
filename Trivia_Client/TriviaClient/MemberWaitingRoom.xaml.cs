using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
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
using System.Collections.ObjectModel;

namespace TriviaClient
{
    public partial class MemberWaitingRoom : Window, INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;
        public static string LoggedUsername => Communicator.GetLoggedUsername();

        private string _roomName;
        public string RoomName
        {
            get => _roomName;
            set
            {
                _roomName = value;
                OnPropertyChanged();
            }
        }

        private string _numOfQuestions;
        public string NumOfQuestions
        {
            get => _numOfQuestions;
            set
            {
                _numOfQuestions = value;
                OnPropertyChanged();
            }
        }

        private string _timePerQuestion;
        public string TimePerQuestion
        {
            get => _timePerQuestion;
            set
            {
                _timePerQuestion = value;
                OnPropertyChanged();
            }
        }

        private ObservableCollection<string> _players = new ObservableCollection<string>();
        public ObservableCollection<string> Players
        {
            get => _players;
            set
            {
                _players = value;
                OnPropertyChanged();
            }
        }

        private CancellationTokenSource _cancellationTokenSource;

        public MemberWaitingRoom(string roomName)
        {
            InitializeComponent();
            DataContext = this;

            RoomName = "Room Name: " + roomName;

            GetRoomStateResponse serverResponse = Communicator.GetRoomState();
            if (serverResponse.status == 1)
            {
                NumOfQuestions = "Number of Questions: " + serverResponse.questionCount;
                TimePerQuestion = "Time for a Question: " + serverResponse.answerTimeout;
                Players = new ObservableCollection<string>(serverResponse.players);
            }

            _cancellationTokenSource = new CancellationTokenSource();
            _ = GetRoomStateAsync(_cancellationTokenSource.Token);
        }

        private async Task GetRoomStateAsync(CancellationToken cancellationToken)
        {
            try
            {
                // In this we always refresh the changes that happened in the room
                while (!cancellationToken.IsCancellationRequested)
                {
                    GetRoomStateResponse serverResponse = Communicator.GetRoomState();

                    if (serverResponse.status == 0) // Error
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            if (Communicator.LeaveRoom())
                            {
                                Menu menuWindow = new Menu();
                                menuWindow.Show();
                                this.Close();
                            }
                        }));
                        return;
                    }

                    // If admin closed the room then member users have to leave
                    if (serverResponse.isActive == false)
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            Menu menuWindow = new Menu();
                            menuWindow.Show();
                            this.Close();
                        }));
                        return;
                    }
                    // If admin started the game then memeber should also join in the game
                    else if (serverResponse.hasGameBegun == true) // Start Game
                    {
                        return; // Move to game window logic here V4.0.0
                    }

                    // Still in waiting room, then we refesh changes to check for new/left memebers
                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        Players = new ObservableCollection<string>(serverResponse.players);
                    }));

                    // Every 3 second do
                    await Task.Delay(3000, cancellationToken);
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

            // Cancel the ongoing task
            _cancellationTokenSource.Cancel();
        }

        private void Border_MouseDown(object sender, MouseButtonEventArgs e)
        {
            this.DragMove();
        }

        private void CloseImage_MouseUp(object sender, MouseButtonEventArgs e)
        {
            if (Communicator.GetIsCreated())
            {
                Communicator.LeaveRoom();
                Communicator.Logout();
            }

            Application.Current.Shutdown();
        }

        private async void LeaveRoom(object sender, RoutedEventArgs e)
        {
            if (Communicator.LeaveRoom())
            {
                // Cancel the ongoing task
                _cancellationTokenSource.Cancel();

                await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                {
                    Menu menuWindow = new Menu();
                    menuWindow.Show();
                    this.Close();
                }));
            }
        }

        protected void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
