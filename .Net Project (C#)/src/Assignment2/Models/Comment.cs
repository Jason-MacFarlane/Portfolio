using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.EntityFrameworkCore;
using System.ComponentModel.DataAnnotations;
using System.Threading.Tasks;
using System.ComponentModel.DataAnnotations.Schema;

namespace Assignment2.Models
{
    public class Comment
    {


        [Key]
        public int CommentId
        {
            get;
            set;
        }

        [Required]
        [ForeignKey("UserId")]
        public int UserId
        {
            get;
            set;
        }

        [Required]
        [StringLength(2048)]
        public string Content
        {
            get;
            set;
        }

        [Required]
        public int Rating
        {
            get;
            set;
        }

        public User User
        {
            get;
            set;
        }

        [Required]
        [ForeignKey("BlogPostId")]
        public int BlogPostId
        {
            get;
            set;
        }

        public BlogPost BlogPost
        {
            get;
            set;
        }


    }
}
