using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
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
using System.Diagnostics;
using System.Windows;

namespace TriviaClient
{
    public partial class Menu : Window
    {
        public static string LoggedUserbame => Communicator.GetLoggedUsername();

        public Menu()
        {
            InitializeComponent();
            DataContext = this; // Sets the data context to this instance of Menu
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
                Communicator.Logout();
            }
            Application.Current.Shutdown();
        }

        private void MoveToCreateRoom(object sender, RoutedEventArgs e)
        {
            CreateRoom createRoomWindow = new CreateRoom();
            createRoomWindow.Show();
            this.Close();
        }

        private void MoveToJoinRoom(object sender, RoutedEventArgs e)
        {
            JoinRoom joinRoomWindow = new JoinRoom();
            joinRoomWindow.Show();
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
    }
}
