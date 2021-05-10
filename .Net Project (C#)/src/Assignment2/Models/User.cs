using System;
using System.Collections.Generic;
using Microsoft.EntityFrameworkCore;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;
using System.ComponentModel.DataAnnotations.Schema;

namespace Assignment2.Models
{
    public class User
    {
        [Key]
        public int UserId
        {
            get;
            set;
        }

        [Required]
        [ForeignKey("RoleId")]
        public int RoleId
        {
            get;
            set;
        }

        [Required]
        [StringLength(50)]
        public string FirstName
        {
            get;
            set;
        }

        [Required]
        [StringLength(50)]
        public string LastName
        {
            get;
            set;
        }

        [Required]
        [StringLength(50)]
        public string EmailAddress
        {
            get;
            set;
        }

        [Required]
        [StringLength(50)]
        public string Password
        {
            get;
            set;
        }

        [Required]
        [DataType(DataType.Date)]
        public DateTime DateOfBirth
        {
            get;
            set;
        }

        [Required]
        [StringLength(50)]
        public string City
        {
            get;
            set;
        }

        [Required]
        [StringLength(50)]
        public string Address
        {
            get;
            set;
        }

        [Required]
        [StringLength(50)]
        public string PostalCode
        {
            get;
            set;
        }

        [Required]
        [StringLength(50)]
        public string Country
        {
            get;
            set;
        }

        public List<BlogPost> BlogPosts
        {
            get;
            set;
        }

    }
}
