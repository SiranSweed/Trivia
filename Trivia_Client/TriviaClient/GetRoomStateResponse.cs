using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TriviaClient
{

    public class GetRoomStateResponse
    {
        public int status { get; set; }
        public bool hasGameBegun { get; set; }
        public bool isActive { get; set; }
        public List<string> players { get; set; }
        public int questionCount { get; set; }
        public int answerTimeout { get; set; }
    }
}
