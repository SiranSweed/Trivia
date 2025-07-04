using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Net;
using System.Windows;
using System.Security.Cryptography;

namespace TriviaClient
{
    public static class Communicator
    {
        private static TcpClient m_socket;
        private static NetworkStream m_clientStream;
        private static bool m_isCreated = false;
        private static string m_loggedUsername = string.Empty;

        // Ctor
        static Communicator()
        {
            try
            {
                m_socket = new TcpClient("127.0.0.1", 4250);
                m_clientStream = m_socket.GetStream();
                m_isCreated = true;
            }
            catch (Exception)
            {
                MessageBox.Show("Error occured while initation the Communicator");
                Application.Current.Shutdown();
            }
        }

        public static bool GetIsCreated()
        { return m_isCreated; }

        public static string GetLoggedUsername()
        { return m_loggedUsername; }

        public static void SetLoggedUsername(string username)
        { m_loggedUsername = username; }

        /*
        This function read a 1 byte message code from the socket with the server and converts it to int
        Inputs: none
        Output: the converted to int type code
        */
        public static int GetMsgCodeFromSock()
        {
            byte[] msgCode = new byte[4];

            m_clientStream.Read(msgCode, 0, 1);

            return BitConverter.ToInt32(msgCode, 0);
        }

        /*
        This function read a 4 bytes data size to read the data itself 
        Inputs: none
        Outputs: the data size in int
        */
        public static int GetDataSizeFromSock()
        {
            byte[] datasSizeField = new byte[4];

            m_clientStream.Read(datasSizeField, 0, 4);
            Array.Reverse(datasSizeField);  // Reverses the buffer cause ToInt32() converts by little-endian format

            return BitConverter.ToInt32(datasSizeField, 0);
        }

        /*
        This function reads data from the socket by using the data size passed to the function
        Inputs: The data size to read.
        Output: The data read from the socket as a bytes array.
        */
        public static byte[] GetContentFromSock(int dataSize)
        {
            byte[] contentField = new byte[dataSize];

            m_clientStream.Read(contentField, 0, dataSize);

            return contentField;
        }

        /*
         * This function send a login request to the server, and receives the server's response
         * Inputs: the username, passwird to login
         * Output: bool - if the login request succeed or didnt.
        */
        public static bool Login(string username, string password)
        {
            byte[] loginReq = Serializer.SerializeRequest(new LoginRequest { username = username, password = password });
            m_clientStream.Write(loginReq, 0, loginReq.Length);
            m_clientStream.Flush();

            int responseCode = GetMsgCodeFromSock();
            int dataSize = GetDataSizeFromSock();
            byte[] content = GetContentFromSock(dataSize);

            // Check if an error response was received
            if(responseCode == (int)ResponseCodes.ERROR_RESP_CODE)
            {
                ErrorResponse deserializedContent = Deserializer.DeserializeErrorResponse(content);
                MessageBox.Show(deserializedContent.message);
                return false;
            }

            return true;
        }

        /*
        This function sends a signup request to the server, and receives the server's response
        Inputs: the usernane password and email to signup
        Output: bool - if the signup request succeed or didnt
        */
        public static bool Signup(string username, string password, string email)
        {
            byte[] signupReq = Serializer.SerializeRequest(new SignupRequest { username = username, password = password, email = email });
            m_clientStream.Write(signupReq, 0, signupReq.Length);
            m_clientStream.Flush();

            int responseCode = GetMsgCodeFromSock();
            int dataSize = GetDataSizeFromSock();
            byte[] content = GetContentFromSock(dataSize);

            // Check if an error response received
            if (responseCode == (int)ResponseCodes.ERROR_RESP_CODE)
            {
                ErrorResponse deserializedContent = Deserializer.DeserializeErrorResponse(content);
                MessageBox.Show(deserializedContent.message);
                return false;
            }

            return true;
        }

        /*
       This function  sends a logout request to the server, and receives the server's response.
       Inputs: none.
       Output: bool - Whether the logout succeed or didnt.
       */
        public static bool Logout()
        {
            byte[] logoutReq = new byte[5];
            logoutReq[0] = (byte)RequestCodes.LOGOUT_REQ_CODE;
            m_clientStream.Write(logoutReq, 0, logoutReq.Length);
            m_clientStream.Flush();

            int responseCode = GetMsgCodeFromSock();
            int dataSize = GetDataSizeFromSock();
            byte[] content = GetContentFromSock(dataSize);

            // Checks if an error response was received
            if (responseCode == (int)ResponseCodes.ERROR_RESP_CODE)
            {
                ErrorResponse deserializedContent = Deserializer.DeserializeErrorResponse(content);
                MessageBox.Show(deserializedContent.message);
            }

            return true;
        }

        /*
       This function sends a joinRoom request to the server, and receives the server's response.
       Inputs: The id of the room to join to.
       Output: bool - Whether the joinRoom succeed or didnt.
       */
        public static bool JoinRoom(int roomId)
        {
            byte[] joinRoomReq = Serializer.SerializeRequest(new JoinRoomRequest { roomId = roomId });
            m_clientStream.Write(joinRoomReq, 0, joinRoomReq.Length);
            m_clientStream.Flush();

            int responseCode = GetMsgCodeFromSock();
            int dataSize = GetDataSizeFromSock();
            byte[] content = GetContentFromSock(dataSize);

            // Checks if an error response was received
            if (responseCode == (int)ResponseCodes.ERROR_RESP_CODE)
            {
                ErrorResponse deserializedContent = Deserializer.DeserializeErrorResponse(content);
                MessageBox.Show(deserializedContent.message);

                return false;
            }

            // If it's not an error response, it's must be a "status: 1" joinRoom response
            return true;
        }

        /*
        This function sends a createRoom request to the server, and receives the server's response
        Inputs: the room's attributes : name, max users, number of questionns, time per question
        Output: bool - if the request succeed or didnt.
        */
        public static bool CreateRoom(string roomName, int maxUsers, int questionCount, int answerTimeout)
        {
            byte[] createRoomReq = Serializer.SerializeRequest(new CreateRoomRequest { roomName = roomName, maxUsers = maxUsers, questionCount = questionCount, answerTimeout = answerTimeout });
            m_clientStream.Write(createRoomReq, 0, createRoomReq.Length);
            m_clientStream.Flush();


            int responseCode = GetMsgCodeFromSock();
            int dataSize = GetDataSizeFromSock();
            byte[] content = GetContentFromSock(dataSize);

            // Checks if an error response was received
            if (responseCode == (int)ResponseCodes.ERROR_RESP_CODE)
            {
                ErrorResponse deserializedContent = Deserializer.DeserializeErrorResponse(content);
                MessageBox.Show(deserializedContent.message);

                return false;
            }

            // If it's not an error response, it's must be a "status: 1" createRoom response
            return true;
        }

        /*
        This function sends a getRooms request, and receives the server's response
        Inputs: none
        Output: the server's response
        */
        public static GetRoomsResponse GetRooms()
        {
            byte[] getRoomsReq = new byte[5];
            getRoomsReq[0] = (byte)RequestCodes.GET_ROOMS_REQ_CODE;
            m_clientStream.Write(getRoomsReq, 0, getRoomsReq.Length);
            m_clientStream.Flush();

            int respoonseCode = GetMsgCodeFromSock();
            int dataSize = GetDataSizeFromSock();
            byte[] content = GetContentFromSock(dataSize);

            // Checks if an error response was received
            if (respoonseCode == (int)ResponseCodes.ERROR_RESP_CODE)
            {
                ErrorResponse deserializedContent = Deserializer.DeserializeErrorResponse(content);
                MessageBox.Show(deserializedContent.message);

                // Returns a GetRoomsResponse with a status that indicates an error
                return new GetRoomsResponse { status = 0, rooms = new List<Room>() };
            }

            return Deserializer.DeserializeGetRoomsResponse(content);
        }

        /*
         * This function send a get playersInRoom request and receives the server's response
         Inputs: the roomId
         Output: the server repsonse content
        */
        public static GetPlayersInRoomResponse GetPlayersInRoom(int roomId)
        {
            byte[] getRoomsReq = Serializer.SerializeRequest(new GetPlayersInRoomRequest { roomId = roomId });
            m_clientStream.Write(getRoomsReq, 0, getRoomsReq.Length);
            m_clientStream.Flush();

            int responseCode = GetMsgCodeFromSock();
            int dataSize = GetDataSizeFromSock();
            byte[] content = GetContentFromSock(dataSize);

            // Checks if an error response was received
            if (responseCode == (int)ResponseCodes.ERROR_RESP_CODE)
            {
                ErrorResponse deserializedContent = Deserializer.DeserializeErrorResponse(content);
                MessageBox.Show(deserializedContent.message);

                // Returns a GetPlayersInRoomResponse with an empty players list
                return new GetPlayersInRoomResponse { players = new List<string>() };
            }

            return Deserializer.DeserializeGetPlayersInRoomResponse(content);
        }

        /*
       This function sends a get high scores requests, and receives the server's response
       Inputs: None.
       Output: the server's response content
       */
        public static GetHighScoresResponse GetHighScores()
        {
            byte[] getHighScoresReq = new byte[5];
            getHighScoresReq[0] = (byte)RequestCodes.GET_HIGH_SCORES_REQ_CODE;
            m_clientStream.Write(getHighScoresReq, 0, getHighScoresReq.Length);
            m_clientStream.Flush();

            int responseCode = GetMsgCodeFromSock();
            int dataSize = GetDataSizeFromSock();
            byte[] content = GetContentFromSock(dataSize);

            // Checks if an error response was received
            if (responseCode == (int)ResponseCodes.ERROR_RESP_CODE)
            {
                ErrorResponse deserializedContent = Deserializer.DeserializeErrorResponse(content);
                MessageBox.Show(deserializedContent.message);

                // Returns a GetHighScoresResponse with an empty statistics list
                return new GetHighScoresResponse { status = 0, statistics = new List<string>() };
            }

            return Deserializer.DeserializeGetHighScoresResponse(content);
        }

        /*
       This function sends a statistics request to the server, and received their response
       Inputs: None.
       Output: the server's response
       */
        public static GetStatisticsResponse GetStatistics()
        {
            byte[] getStatisticsReq = new byte[5];
            getStatisticsReq[0] = (byte)RequestCodes.GET_STATS_REQ_CODE;
            m_clientStream.Write(getStatisticsReq, 0, getStatisticsReq.Length);
            m_clientStream.Flush();

            int responseCode = GetMsgCodeFromSock();
            int dataSize = GetDataSizeFromSock();
            byte[] content = GetContentFromSock(dataSize);

            // Checks if an error response was received
            if (responseCode == (int)ResponseCodes.ERROR_RESP_CODE)
            {
                ErrorResponse deserializedContent = Deserializer.DeserializeErrorResponse(content);
                MessageBox.Show(deserializedContent.message);

                // Returns a GetStatisticsResponse with an empty statistics list
                return new GetStatisticsResponse { status = 0, statistics = new List<Statistic>() };
            }

            return Deserializer.DeserializeGetStatisticsResponse(content);
        }

        /*
        This function operates a sending of a closeRoom request to the server,
        and receives the server's response.
        Inputs: None.
        Output: bool - Whether the closeRoom succeed or not.
        */
        public static bool CloseRoom()
        {
            byte[] closeRoomReq = new byte[5];
            closeRoomReq[0] = (byte)RequestCodes.CLOSE_ROOM_REQ_CODE;
            m_clientStream.Write(closeRoomReq, 0, closeRoomReq.Length);
            m_clientStream.Flush();

            int respCode = GetMsgCodeFromSock();
            int dataSize = GetDataSizeFromSock();
            byte[] content = GetContentFromSock(dataSize);

            // Checks if an error response was received
            if (respCode == (int)ResponseCodes.ERROR_RESP_CODE)
            {
                ErrorResponse deserializedContent = Deserializer.DeserializeErrorResponse(content);
                MessageBox.Show(deserializedContent.message);

                return false;
            }

            return true;
        }

        /*
        This function operates a sending of a closeRoom request to the server,
        and receives the server's response.
        Inputs: None.
        Output: bool - Whether the closeRoom succeed or not.
        */
        public static bool LeaveRoom()
        {
            byte[] leaveRoomReq = new byte[5];
            leaveRoomReq[0] = (byte)RequestCodes.LEAVE_ROOM_REQ_CODE;
            m_clientStream.Write(leaveRoomReq, 0, leaveRoomReq.Length);
            m_clientStream.Flush();

            int respCode = GetMsgCodeFromSock();
            int dataSize = GetDataSizeFromSock();
            byte[] content = GetContentFromSock(dataSize);

            // Checks if an error response was received
            if (respCode == (int)ResponseCodes.ERROR_RESP_CODE)
            {
                ErrorResponse deserializedContent = Deserializer.DeserializeErrorResponse(content);
                MessageBox.Show(deserializedContent.message);

                return false;
            }

            return true;
        }

        /*
        This function operates a sending of a getRoomState request to the server,
        and receives the server's response.
        Inputs: None.
        Output: bool - Whether the getRoomState succeed or not.
        */
        public static GetRoomStateResponse GetRoomState()
        {
            byte[] getRoomStateReq = new byte[5];
            getRoomStateReq[0] = (byte)RequestCodes.GET_ROOM_STATE_REQ_CODE;
            m_clientStream.Write(getRoomStateReq, 0, getRoomStateReq.Length);
            m_clientStream.Flush();

            int respCode = GetMsgCodeFromSock();
            int dataSize = GetDataSizeFromSock();
            byte[] content = GetContentFromSock(dataSize);

            // Checks if an error response was received
            if (respCode == (int)ResponseCodes.ERROR_RESP_CODE)
            {
                ErrorResponse deserializedContent = Deserializer.DeserializeErrorResponse(content);
                MessageBox.Show(deserializedContent.message);

                return new GetRoomStateResponse { status = 0, hasGameBegun = false, isActive = false, players = new List<string>(), questionCount = 0, answerTimeout = 0 };
            }
            else if (respCode == (int)ResponseCodes.START_GAME_RESP_CODE)
            {
                MessageBox.Show("GAME STARTED!!! (V4.0.0)");

                return new GetRoomStateResponse { status = 2, hasGameBegun = true, isActive = true, players = new List<string>(), questionCount = 0, answerTimeout = 0 };
            }

            return Deserializer.DeserializeGetRoomStateResponse(content);
        }
    }
}
