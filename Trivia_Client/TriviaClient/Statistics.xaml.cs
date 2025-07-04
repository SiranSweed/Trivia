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
    public partial class Statistics : Window
    {
        public static string LoggedUsername => Communicator.GetLoggedUsername();
        public Statistics()
        {
            InitializeComponent();
            DataContext = this;
            LoadStatistics();
        }

        private void LoadStatistics()
        {
            GetStatisticsResponse serverResponse = Communicator.GetStatistics();
            List<Statistic> statistics = serverResponse.statistics;

            if (serverResponse.status == 0)
            {
                Menu menuWindow = new Menu();
                menuWindow.Show();
                this.Close();
                return;
            }

            if (statistics.Count == 1)
            {
                NumberOfGamesTextBlock.Text = $"Number of Games: {statistics[0].num_of_player_games.ToString()}";
                CorrectAnswersTextBlock.Text = $"Number of Correct Answers: {statistics[0].num_of_correct_answers.ToString()}";
                WrongAnswersTextBlock.Text = $"Number of Wrong Answers: {statistics[0].num_of_wrong_answers.ToString()}";
                AverageTimeTextBlock.Text = $"Average Time for Answer: {statistics[0].avg_answer_time.ToString()}";
            }

            else
            {
                MessageBox.Show("Failed to load statistics.");
            }
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
