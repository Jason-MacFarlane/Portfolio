using System;
using System.Collections.Generic;
using System.Linq;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Threading.Tasks;

namespace Assignment2.Models
{
    public class BadWord
    {

        [Key]
        public int BadWordId
        {
            get;
            set;
        }

        [StringLength(50)]
        public string Word
        {
            get;
            set;
        }

    }
}
