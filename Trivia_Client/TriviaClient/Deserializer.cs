using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TriviaClient
{
    public static class Deserializer
    {
     
        public static ErrorResponse DeserializeErrorResponse(byte[] buffer)
        {
            ErrorResponse errorResp = JsonConvert.DeserializeObject<ErrorResponse>(Encoding.UTF8.GetString(buffer));

            return errorResp;
        }
  
        public static GetRoomsResponse DeserializeGetRoomsResponse(byte[] buffer)
        {
            GetRoomsResponse getRoomsResp = JsonConvert.DeserializeObject<GetRoomsResponse>(Encoding.UTF8.GetString(buffer));

            return getRoomsResp;
        }
        public static GetPlayersInRoomResponse DeserializeGetPlayersInRoomResponse(byte[] buffer)
        {
            GetPlayersInRoomResponse getPlayersInRoomResp = JsonConvert.DeserializeObject<GetPlayersInRoomResponse>(Encoding.UTF8.GetString(buffer));

            return getPlayersInRoomResp;
        }
        public static GetHighScoresResponse DeserializeGetHighScoresResponse(byte[] buffer)
        {
            GetHighScoresResponse getHighScoresResp = JsonConvert.DeserializeObject<GetHighScoresResponse>(Encoding.UTF8.GetString(buffer));

            return getHighScoresResp;
        }
        public static GetStatisticsResponse DeserializeGetStatisticsResponse(byte[] buffer)
        {
            GetStatisticsResponse getStatisticsResp = JsonConvert.DeserializeObject<GetStatisticsResponse>(Encoding.UTF8.GetString(buffer));

            return getStatisticsResp;
        }
        public static GetRoomStateResponse DeserializeGetRoomStateResponse(byte[] buffer)
        {
            GetRoomStateResponse getStatisticsResp = JsonConvert.DeserializeObject<GetRoomStateResponse>(Encoding.UTF8.GetString(buffer));

            return getStatisticsResp;
        }


    }
}

