using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Newtonsoft.Json;
using System.Threading.Tasks;
using System.Xml;

namespace TriviaClient
{
    public static class Serializer
    {
        public static byte[] SerializeRequest(LoginRequest request)
        {
            string jsonString = JsonConvert.SerializeObject(request);

            byte[] jsonBytes = Encoding.UTF8.GetBytes(jsonString);

            int dataSize = jsonBytes.Length;

            byte[] result = new byte[1 + 4 + dataSize];

            result[0] = (byte)RequestCodes.LOGIN_REQ_CODE;

            // Sets the next 4 bytes to the  data size (in little-endian order)
            byte[] dataSizeBytes = BitConverter.GetBytes(dataSize);
            Array.Copy(dataSizeBytes, 0, result, 1, 4);

            // Copy the JSON data bytes to the result array starting at index 5
            Array.Copy(jsonBytes, 0, result, 5, dataSize);

            return result;
        }

        public static byte[] SerializeRequest(SignupRequest request)
        {
            string jsonString = JsonConvert.SerializeObject(request);

            byte[] jsonBytes = Encoding.UTF8.GetBytes(jsonString);

            int dataSize = jsonBytes.Length;

            byte[] result = new byte[1 + 4 + dataSize];

            result[0] = (byte)RequestCodes.SIGNUP_REQ_CODE;

            // Sets the next 4 bytes to the data size (in little-endian order)
            byte[] dataSizeBytes = BitConverter.GetBytes(dataSize);
            Array.Copy(dataSizeBytes, 0, result, 1, 4);

            // Copys the JSON data bytes to the result array starting at index 5
            Array.Copy(jsonBytes, 0, result, 5, dataSize);

            return result;
        }

        public static byte[] SerializeRequest(JoinRoomRequest request)
        {
            string jsonString = JsonConvert.SerializeObject(request);

            byte[] jsonBytes = Encoding.UTF8.GetBytes(jsonString);

            int dataSize = jsonBytes.Length;

            byte[] result = new byte[1 + 4 + dataSize];

            result[0] = (byte)RequestCodes.JOIN_ROOM_REQ_CODE;

            // Sets the next 4 bytes to the data size (in little-endian order)
            byte[] dataSizeBytes = BitConverter.GetBytes(dataSize);
            Array.Copy(dataSizeBytes, 0, result, 1, 4);

            // Copys the JSON data bytes to the result array starting at index 5
            Array.Copy(jsonBytes, 0, result, 5, dataSize);

            return result;
        }

        public static byte[] SerializeRequest(CreateRoomRequest request)
        {
            string jsonString = JsonConvert.SerializeObject(request);

            byte[] jsonBytes = Encoding.UTF8.GetBytes(jsonString);

            int dataSize = jsonBytes.Length;

            byte[] result = new byte[1 + 4 + dataSize];

            result[0] = (byte)RequestCodes.CREATE_ROOM_REQ_CODE;

            // Sets the next 4 bytes to the data size (in little-endian order)
            byte[] dataSizeBytes = BitConverter.GetBytes(dataSize);
            Array.Copy(dataSizeBytes, 0, result, 1, 4);

            // Copys the JSON data bytes to the result array starting at index 5
            Array.Copy(jsonBytes, 0, result, 5, dataSize);

            return result;
        }

        public static byte[] SerializeRequest(GetPlayersInRoomRequest request)
        {
            string jsonString = JsonConvert.SerializeObject(request);

            byte[] jsonBytes = Encoding.UTF8.GetBytes(jsonString);

            int dataSize = jsonBytes.Length;

            byte[] result = new byte[1 + 4 + dataSize];

            result[0] = (byte)RequestCodes.JOIN_ROOM_REQ_CODE;

            // Sets the next 4 bytes to the data size (in little-endian order)
            byte[] dataSizeBytes = BitConverter.GetBytes(dataSize);
            Array.Copy(dataSizeBytes, 0, result, 1, 4);

            // Copys the JSON data bytes to the result array starting at index 5
            Array.Copy(jsonBytes, 0, result, 5, dataSize);

            return result;
        }
    }
}
