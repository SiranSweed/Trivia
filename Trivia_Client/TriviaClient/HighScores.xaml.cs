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

namespace TriviaClient
{
    public partial class HighScores : Window
    {
        public static string LoggedUsername => Communicator.GetLoggedUsername();
        public HighScores()
        {
            InitializeComponent();
            DataContext = this;
            LoadHighScores();
        }

        private void LoadHighScores()
        {
            GetHighScoresResponse serverResponse = Communicator.GetHighScores();
            List<string> statistics = serverResponse.statistics;

            if (serverResponse.status == 0)
            {
                Menu menuWindow = new Menu();
                menuWindow.Show();
                this.Close();
                return;
            }

            FirstTextBlock.Text = statistics.Count > 0 ? $"1st Place: {statistics[0]}" : $"1st Place: [...]";
            SecondTextBlock.Text = statistics.Count > 1 ? $"2nd Place: {statistics[1]}" : $"2nd Place: [...]";
            ThirdTextBlock.Text = statistics.Count > 2 ? $"3th Place: {statistics[2]}" : $"3th Place: [...]";
            FourthTextBlock.Text = statistics.Count > 3 ? $"4th Place: {statistics[3]}" : $"4th Place: [...]";
            FifthTextBlock.Text = statistics.Count > 4 ? $"5th Place: {statistics[4]}" : $"5th Place: [...]";
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
