using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace Assignment2.Models
{
    public class Photo
    {
        [Key]
        public int PhotoId
        {
            get;
            set;
        }

        [Required]
        public int BlogPostId
        {
            get;
            set;
        }

        [Required]
        [StringLength(255)]
        public string FileName
        {
            get;
            set;
        }

        [Required]
        [StringLength(2048)]
        public string Url
        {
            get;
            set;
        }



    }
}
