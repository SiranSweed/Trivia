using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TriviaClient
{
    public class Room
    {
        public int id { get; set; }
        public string name { get; set; }
    }
    public class GetRoomsResponse
    {
        public int status { get; set; }

        public List<Room> rooms { get; set; }
    }
}
