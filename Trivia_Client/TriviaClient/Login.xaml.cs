using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace TriviaClient
{
    public partial class Login : Window
    {
        public Login()
        {
            InitializeComponent();
        }

        private void TextUsername_MouseDown(object sender, MouseButtonEventArgs e)
        {
            txtUsername.Focus();
        }

        private void TxtUsername_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (!string.IsNullOrEmpty(txtUsername.Text) && txtUsername.Text.Length > 0)
            {
                textUsername.Visibility = Visibility.Collapsed;
            }
            else
            {
                textUsername.Visibility = Visibility.Visible;
            }
        }

        private void TextPassword_MouseDown(object sender, MouseButtonEventArgs e)
        {
            txtPassword.Focus();
        }

        private void TxtPassword_PasswordChanged(object sender, RoutedEventArgs e)
        {
            if (!string.IsNullOrEmpty(txtPassword.Password) && txtPassword.Password.Length > 0)
            {
                textPassword.Visibility = Visibility.Collapsed;
            }
            else
            {
                textPassword.Visibility = Visibility.Visible;
            }
        }
        private void ShowPasswordCheckBox_Checked(object sender, RoutedEventArgs e)
        {
            txtVisiblePassword.Visibility = Visibility.Visible;
            txtPassword.Visibility = Visibility.Collapsed;
            txtVisiblePassword.Text = txtPassword.Password;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (!string.IsNullOrEmpty(txtUsername.Text) && !string.IsNullOrEmpty(txtPassword.Password))
            {
                if (Communicator.Login(txtUsername.Text, txtPassword.Password))
                {
                    Communicator.SetLoggedUsername(txtUsername.Text);

                    // For valid login details - moves to the menu
                    Menu menuWindow = new Menu();
                    menuWindow.Show();
                    this.Close();
                }
            }
        }

        private void Border_MouseDown(object sender, MouseButtonEventArgs e)
        {
            this.DragMove();
        }

        private void CloseImage_MouseUp(object sender, MouseButtonEventArgs e)
        {
            if (Communicator.GetIsCreated())
            {
                Communicator.Logout();
            }
            Application.Current.Shutdown();
        }

        private void MoveToSignup(object sender, RoutedEventArgs e)
        {
            Signup signupPage = new Signup();
            signupPage.Show();
            this.Close();
        }

        private void ShowPasswordCheckBox_Unchecked(object sender, RoutedEventArgs e)
        {
            txtVisiblePassword.Visibility = Visibility.Collapsed;
            txtPassword.Visibility = Visibility.Visible;
            txtPassword.Password = txtVisiblePassword.Text;
        }
    }
}
