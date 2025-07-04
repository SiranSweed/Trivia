using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TriviaClient
{
    public class GetHighScoresResponse
    {
        public int status { get; set; }
        public List<string> statistics { get; set; }
    }
}
