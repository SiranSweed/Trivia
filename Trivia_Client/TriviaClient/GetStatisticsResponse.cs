using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TriviaClient
{
    public class Statistic
    {
        public int num_of_player_games { get; set; }
        public int num_of_correct_answers { get; set; }
        public int num_of_wrong_answers { get; set; }
        public double avg_answer_time { get; set; }
    }
    public class GetStatisticsResponse
    {
        public int status { get; set; }
        public List<Statistic> statistics { get; set; }
    }
}
