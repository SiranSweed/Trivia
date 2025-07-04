using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
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

namespace TriviaClient
{
    public partial class CreateRoom : Window
    {
        public static string LoggedUsername => Communicator.GetLoggedUsername();
        public CreateRoom()
        {
            InitializeComponent();
            DataContext = this;
        }

        private static readonly Regex m_onlyNaturalsPattern = new Regex("^[1-9]\\d*$");
        private static bool IsNaturalNum(string text)
        {
            return m_onlyNaturalsPattern.IsMatch(text);
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

        private void TextRoomname_MouseDown(object sender, MouseButtonEventArgs e)
        {
            txtRoomname.Focus();
        }

        private void TxtRoomname_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (!string.IsNullOrEmpty(txtRoomname.Text) && txtRoomname.Text.Length > 0)
            {
                textRoomname.Visibility = Visibility.Collapsed;
            }
            else
            {
                textRoomname.Visibility = Visibility.Visible;
            }
        }

        private void TextNumofplayers_MouseDown(object sender, MouseButtonEventArgs e)
        {
            txtNumofplayers.Focus();
        }

        private void TxtNumofplayers_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (!string.IsNullOrEmpty(txtNumofplayers.Text) && txtNumofplayers.Text.Length > 0)
            {
                textNumofplayers.Visibility = Visibility.Collapsed;
            }
            else
            {
                textNumofplayers.Visibility = Visibility.Visible;
            }
        }

        private void TextNumofquestions_MouseDown(object sender, MouseButtonEventArgs e)
        {
            txtNumofquestions.Focus();
        }

        private void TxtNumofquestions_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (!string.IsNullOrEmpty(txtNumofquestions.Text) && txtNumofquestions.Text.Length > 0)
            {
                textNumofquestions.Visibility = Visibility.Collapsed;
            }
            else
            {
                textNumofquestions.Visibility = Visibility.Visible;
            }
        }

        private void TextTimeforquestion_MouseDown(object sender, MouseButtonEventArgs e)
        {
            txtTimeforquestion.Focus();
        }

        private void TxtTimeforquestion_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (!string.IsNullOrEmpty(txtTimeforquestion.Text) && txtTimeforquestion.Text.Length > 0)
            {
                textTimeforquestion.Visibility = Visibility.Collapsed;
            }
            else
            {
                textTimeforquestion.Visibility = Visibility.Visible;
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (!string.IsNullOrEmpty(txtRoomname.Text) && !string.IsNullOrEmpty(txtNumofplayers.Text) && !string.IsNullOrEmpty(txtNumofquestions.Text) && !string.IsNullOrEmpty(txtTimeforquestion.Text)
                && IsNaturalNum(txtNumofplayers.Text) && IsNaturalNum(txtNumofquestions.Text) && IsNaturalNum(txtTimeforquestion.Text))
            {
                if (Communicator.CreateRoom(txtRoomname.Text, Int32.Parse(txtNumofplayers.Text), Int32.Parse(txtNumofquestions.Text), Int32.Parse(txtTimeforquestion.Text)))
                {
                    MessageBox.Show("Room Created Succsfully");
                    AdminWaitingRoom waitingRoomWindow = new AdminWaitingRoom(txtRoomname.Text, txtNumofplayers.Text, txtNumofquestions.Text, txtTimeforquestion.Text);
                    waitingRoomWindow.Show();
                    this.Close();
                }
            }
            else
            {
                MessageBox.Show("The last 3 fields have to be positive integers");
            }
        }
    }
}
