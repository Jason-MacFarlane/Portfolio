using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Http;
using Assignment2.Models;
using System.Globalization;
using System.Text.RegularExpressions;
using Microsoft.WindowsAzure.Storage;
using System.IO;
using Microsoft.WindowsAzure.Storage.Blob;

// For more information on enabling MVC for empty projects, visit http://go.microsoft.com/fwlink/?LinkID=397860

namespace Assignment1.Controllers.Home
{
    public class HomeController : Controller
    {
        private Assignment2DataContext _dataContext;

        public HomeController(Assignment2DataContext context)
        {
            _dataContext = context;
        }

        public IActionResult Index()
        {
            var blogPostPhotos = _dataContext.Photos.ToList();
            ViewData["Photos"] = blogPostPhotos;

            return View(_dataContext.BlogPosts.ToList());
        }

        public IActionResult Login()
        {
            return View();
        }

        public IActionResult Register()
        {
            return View();
        }

        public IActionResult AddBlogPost()
        {
            return View();
        }

        public IActionResult EditProfile(int id)
        {
            var userToEdit = (from u in _dataContext.Users where u.UserId == id select u).FirstOrDefault();
            return View(userToEdit);
        }

        public IActionResult DisplayBadWords()
        {
            return View(_dataContext.BadWords.ToList());
        }

        public IActionResult AddBadWord()
        {
            BadWord newBadWord = new BadWord
            {
                Word = Request.Form["word"]
            };

            _dataContext.BadWords.Add(newBadWord);
            _dataContext.SaveChanges();

            return RedirectToAction("DisplayBadWords");
        }

        public IActionResult DeleteBadWord(int id)
        {
            var badWordToDelete = (from b in _dataContext.BadWords where b.BadWordId == id select b).FirstOrDefault();
            _dataContext.BadWords.Remove(badWordToDelete);
            _dataContext.SaveChanges();
            return RedirectToAction("DisplayBadWords");
        }

        public async Task<IActionResult> UploadFileNow(ICollection<IFormFile> files)
        {
            await Upload(files, Convert.ToInt32(Request.Form["BlogPostId"]));

            return RedirectToAction("EditBlogPost", new { id = Convert.ToInt32(Request.Form["BlogPostId"]) });
        }

        public async Task<bool> Upload(ICollection<IFormFile> files, int id)
        {
            var storageAccount = CloudStorageAccount.Parse("DefaultEndpointsProtocol=https;AccountName=macf0080lab5;AccountKey=nb/zAHhtnvpau9/9ptKhpS/IqW2t+uN+uJ5zFqiKGbJXda9WldAhhbccOOSyz+X/sWTIM//UrVwEqbyr5jJtAA==;EndpointSuffix=core.windows.net");

            var blobClient = storageAccount.CreateCloudBlobClient();

            var container = blobClient.GetContainerReference("jasonsphotostorage");
            await container.CreateIfNotExistsAsync();

            var permissions = new BlobContainerPermissions();
            permissions.PublicAccess = BlobContainerPublicAccessType.Blob;
            await container.SetPermissionsAsync(permissions);

            foreach (var file in files)
            {
                try
                {

                    var blockBlob = container.GetBlockBlobReference(file.FileName);
                    if (await blockBlob.ExistsAsync())
                        await blockBlob.DeleteAsync();

                    using (var memoryStream = new MemoryStream())
                    {

                        await file.CopyToAsync(memoryStream);


                        memoryStream.Position = 0;


                        await blockBlob.UploadFromStreamAsync(memoryStream);
                    }

                    var photo = new Photo();
                    photo.Url = blockBlob.Uri.AbsoluteUri;
                    photo.FileName = file.FileName;
                    photo.BlogPostId = id;

                    _dataContext.Photos.Add(photo);
                    _dataContext.SaveChanges();
                }
                catch
                {
                    return false;
                }
            }

            return true;
        }

        public async Task<IActionResult> DeletePhoto(string file, int id)
        {
            await DeletePhotoFromStorage(file, id);

            return RedirectToAction("EditBlogPost",new { id = id });
        }

        public async Task<bool> DeletePhotoFromStorage(string file, int id)
        {
            var storageAccount = CloudStorageAccount.Parse("DefaultEndpointsProtocol=https;AccountName=macf0080lab5;AccountKey=nb/zAHhtnvpau9/9ptKhpS/IqW2t+uN+uJ5zFqiKGbJXda9WldAhhbccOOSyz+X/sWTIM//UrVwEqbyr5jJtAA==;EndpointSuffix=core.windows.net");

            var blobClient = storageAccount.CreateCloudBlobClient();

            var container = blobClient.GetContainerReference("jasonsphotostorage");

            var blockBlob = container.GetBlockBlobReference(file);

            await blockBlob.DeleteIfExistsAsync();

            var fileToDelete = (from f in _dataContext.Photos where f.FileName == file select f).FirstOrDefault();

            _dataContext.Photos.Remove(fileToDelete);
            _dataContext.SaveChanges();

            return true;

        }

        public IActionResult Logout()
        {
            HttpContext.Session.Clear();
            return RedirectToAction("Index");
        }

        public async Task<IActionResult> DeleteBlogPost(int id)
        {
            var blogPostToDelete = (from b in _dataContext.BlogPosts where b.BlogPostId == id select b).FirstOrDefault();
            var commentsToDelete = (from c in _dataContext.Comments where c.BlogPostId == id select c).ToList();
            var photosToDelete = (from p in _dataContext.Photos where p.BlogPostId == id select p).ToList();
            foreach (var comment in commentsToDelete)
            {
                _dataContext.Comments.Remove(comment);
            }
            foreach(var photo in photosToDelete)
            {
                await DeletePhotoFromStorage(photo.FileName, blogPostToDelete.BlogPostId);
            }
            _dataContext.BlogPosts.Remove(blogPostToDelete);
            _dataContext.SaveChanges();
            return RedirectToAction("Index");
        }

        public IActionResult EditBlogPost(int id)
        {
            var blogPostToUpdate = (from b in _dataContext.BlogPosts where b.BlogPostId == id select b).FirstOrDefault();
            var blogPostPhotos = (from p in _dataContext.Photos where p.BlogPostId == id select p).ToList();
            ViewData["Photos"] = blogPostPhotos;
            return View(blogPostToUpdate);
        }

        public IActionResult CreateUser(User user)
        {
            _dataContext.Users.Add(user);
            _dataContext.SaveChanges();
            return RedirectToAction("Login");
        }

        public async Task<IActionResult> CreateBlogPost(BlogPost blogPost, ICollection<IFormFile> files)
        {
            blogPost.UserId = int.Parse(HttpContext.Session.GetString("id"));
            blogPost.Posted = DateTime.Now;
            _dataContext.BlogPosts.Add(blogPost);
            _dataContext.SaveChanges();
            await Upload(files, blogPost.BlogPostId);
            return RedirectToAction("Index");
        }

        [HttpPost]
        public IActionResult LoginUser()
        {
            string email = Request.Form["email"];
            string password = Request.Form["password"];
            var newUser = (from u in _dataContext.Users where u.EmailAddress == email select u).FirstOrDefault();
            if (newUser == null || newUser.Password != password)
            {
                return RedirectToAction("Login");
            }
            HttpContext.Session.SetString("id", newUser.UserId.ToString());
            HttpContext.Session.SetString("firstName", newUser.FirstName);
            HttpContext.Session.SetString("lastName", newUser.LastName);
            HttpContext.Session.SetString("roleId", newUser.RoleId.ToString());
            return RedirectToAction("Index");
        }

        public IActionResult DisplayFullBlogPost(int id)
        {
            var blogToDisplay = (from b in _dataContext.BlogPosts where b.BlogPostId == id select b).FirstOrDefault();
            var user = (from u in _dataContext.Users where u.UserId == blogToDisplay.UserId select u).FirstOrDefault();
            var comments = (from c in _dataContext.Comments where c.BlogPostId == id select c).ToList();
            var commentUsers = (from u in _dataContext.Users join c in _dataContext.Comments on u.UserId equals c.UserId select u).ToList();
            var photos = (from p in _dataContext.Photos where p.BlogPostId == id select p).ToList();
            foreach(var comment in comments)
            {
                Censor(comment);
            }
            ViewData["BlogPost"] = blogToDisplay;
            ViewData["User"] = user;
            ViewData["Comment"] = comments;
            ViewData["CommentUsers"] = commentUsers;
            ViewData["Photos"] = photos;
            return View();
        }

        public void Censor(Comment comment)
        {
            var badWords = _dataContext.BadWords.ToList();

            if (badWords != null)
            {

                string censor = null;
                int wordLength = 0;
                CultureInfo culture = new CultureInfo("en-CA");

                for (int i = 0; i < badWords.Count; i++)
                {

                    censor = "*";
                    wordLength = badWords[i].Word.Length;

                    while (censor.Length != badWords[i].Word.Length)
                    {
                        censor = censor + "*";
                    }

                    if (culture.CompareInfo.IndexOf(comment.Content, badWords[i].Word, CompareOptions.IgnoreCase) != -1)
                    {

                        comment.Content = Regex.Replace(comment.Content, badWords[i].Word, censor, RegexOptions.IgnoreCase);

                    }

                }

            }
        }

        public IActionResult CreateComment()
        {
            var id = Convert.ToInt32(Request.Form["BlogPostId"]);
            var userId = Convert.ToInt32(Request.Form["UserId"]);
            Comment newComment = new Comment()
            {
                BlogPostId = id,
                UserId = userId,
                Content = Request.Form["Content"]
            };


            _dataContext.Comments.Add(newComment);
            _dataContext.SaveChanges();

            return RedirectToAction("DisplayFullBlogPost", new { id = id });
        }

        public async Task<IActionResult> ModifyBlogPost(BlogPost blogPost, ICollection<IFormFile> files)
        {
            
            var id = Convert.ToInt32(Request.Form["BlogPostId"]);
            var blogPostToUpdate = (from b in _dataContext.BlogPosts where b.BlogPostId == id select b).FirstOrDefault();

            blogPostToUpdate.Title = blogPost.Title;
            blogPostToUpdate.Content = blogPost.Content;
            blogPostToUpdate.IsAvailable = blogPost.IsAvailable;
            blogPostToUpdate.ShortDescription = blogPostToUpdate.ShortDescription;
            await Upload(files, id);
            _dataContext.SaveChanges();

            return RedirectToAction("DisplayFullBlogPost", new { id = id });

        }

        public IActionResult EditUser(User user)
        {
            var id = Convert.ToInt32(Request.Form["UserId"]);
            var userToUpdate = (from u in _dataContext.Users where u.UserId == id select u).FirstOrDefault();

            userToUpdate.FirstName = user.FirstName;
            userToUpdate.LastName = user.LastName;
            userToUpdate.RoleId = user.RoleId;
            userToUpdate.EmailAddress = user.EmailAddress;
            userToUpdate.Password = user.Password;
            userToUpdate.DateOfBirth = user.DateOfBirth;
            userToUpdate.City = user.City;
            userToUpdate.Address = user.Address;
            userToUpdate.PostalCode = user.PostalCode;
            userToUpdate.Country = user.Country;

            _dataContext.SaveChanges();

            HttpContext.Session.SetString("id", userToUpdate.UserId.ToString());
            HttpContext.Session.SetString("firstName", userToUpdate.FirstName);
            HttpContext.Session.SetString("lastName", userToUpdate.LastName);
            HttpContext.Session.SetString("roleId", userToUpdate.RoleId.ToString());
            return RedirectToAction("Index");


        }


    }
}
