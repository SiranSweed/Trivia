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

namespace TriviaClient
{
    public partial class AdminWaitingRoom : Window, INotifyPropertyChanged
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

        private string _maxPlayersNum;
        public string MaxPlayersNum
        {
            get => _maxPlayersNum;
            set
            {
                _maxPlayersNum = value;
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

        private List<string> _players;
        public List<string> Players
        {
            get => _players;
            set
            {
                _players = value;
                OnPropertyChanged();
            }
        }

        private CancellationTokenSource _cancellationTokenSource;

        public AdminWaitingRoom(string roomName, string numOfPlayers, string numOfQuestions, string timeForQuestion)
        {
            InitializeComponent();
            DataContext = this;

            RoomName = "Room Name: " + roomName;
            MaxPlayersNum = "Maximum Number of Players: " + numOfPlayers;
            NumOfQuestions = "Number of Questions: " + numOfQuestions;
            TimePerQuestion = "Time for a Question: " + timeForQuestion;

            _cancellationTokenSource = new CancellationTokenSource();
            _ = GetRoomStateAsync(_cancellationTokenSource.Token);
        }

        private async Task GetRoomStateAsync(CancellationToken cancellationToken)
        {
            // Here we also refresh for changes, if new players joined/left and also for game starting
            try
            {
                while (!cancellationToken.IsCancellationRequested)
                {
                    GetRoomStateResponse serverResponse = Communicator.GetRoomState();

                    if (serverResponse.status == 0) // Error
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            Communicator.CloseRoom();
                            Menu menuWindow = new Menu();
                            menuWindow.Show();
                            this.Close();
                        }));
                        return;
                    }

                    if (serverResponse.hasGameBegun == true) // Start Game for V4.0.0 later
                    {
                        // TODO: Move to game window logic here
                        return;
                    }

                    // Still on waiting room, reshresh for changes if players joined/left
                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        Players = serverResponse.players;
                    }));

                    // refesh every 3 second
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

            // Signal the cancellation token to stop
            _cancellationTokenSource.Cancel();
        }

        private void Border_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Left)
            {
                this.DragMove();
            }
        }

        private void CloseImage_MouseUp(object sender, MouseButtonEventArgs e)
        {
            if (Communicator.GetIsCreated())
            {
                Communicator.CloseRoom();
                Communicator.Logout();
            }

            Application.Current.Shutdown();
        }

        private void StartGame(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("TODO: V4.0.0");
        }

        private async void CloseRoom(object sender, RoutedEventArgs e)
        {
            if (Communicator.CloseRoom())
            {
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
