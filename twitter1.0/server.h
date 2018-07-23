
struct user
{
	int userid;
	char *username;
	char *email;
	int followers;
	int following;
	int my_tweet_count;
};

struct tweet
{
	int tid;
	int userid;
	char *username;
	char *tweet_text;
};

struct comment
{
	int cid;
	int tid;
	int userid;
	char *username;
	char *comment_text;
};


char* get_password_from_file(char *);
int name_present_in_db(char *);
void signup_view();
int searchbyname(char *);
void store_in_db(char* username, char* password, char* email);


int storetweet(int userid,char *tweet)
{
	return createTweet(userid, tweet);
}

struct comment **getCommentServers(int tid, int *count)
{
	struct commentsfile** comments_from_db = getComments(tid, count);
	struct comment** tweet_comments = (struct comment**)malloc((*count)*sizeof(struct comment*));
	for (int i = *count - 1; i >= 0; i--)
	{
		struct comment* tweet_comment = (struct comment*)malloc(sizeof(struct comment));
		tweet_comment->cid = comments_from_db[i]->cid;
		tweet_comment->tid = comments_from_db[i]->tid;
		tweet_comment->userid = comments_from_db[i]->userid;
		struct userfile* user = getUserDetails(comments_from_db[i]->userid);
		tweet_comment->username = user->uname;
		tweet_comment->comment_text = comments_from_db[i]->comment;
		tweet_comments[i] = tweet_comment;
	}
	return tweet_comments;
}




struct tweet** showFeed(int userid, int* l)
{
	int *followed_by_me_count = (int*)malloc(sizeof(int));

	int *len2 = (int*)malloc(sizeof(int));
	struct tweetfile** user_tweets = getUserTweets(userid, len2);
	int total_len = *len2;
	struct tweet** all_tweets = (struct tweet**)malloc((total_len)*sizeof(struct tweet*));

	for (int i = 0; i < total_len; i++)
	{
		struct tweet* all_tweet = (struct tweet*)malloc(sizeof(struct tweet));
		all_tweet->tid = user_tweets[i]->tid;
		all_tweet->tweet_text = user_tweets[i]->tweet;
		all_tweet->userid = user_tweets[i]->userid;
		//all_tweet->count_of_retweets_and_likes = user_tweets[i]->count;
		all_tweets[i] = all_tweet;

	}



	struct followersfile** followed_by_me = getUserFollowing(userid, followed_by_me_count);

	for (int j = 0; j < (*followed_by_me_count); j++)
	{
		user_tweets = getUserTweets(followed_by_me[j]->userid, len2);
		total_len = total_len + (*len2);
		all_tweets = (struct tweet**)realloc(all_tweets, total_len*sizeof(struct tweet*));

		for (int i = 0; i < total_len; i++)
		{
			struct tweet* all_tweet = (struct tweet*)malloc(sizeof(struct tweet));
			all_tweet->tid = user_tweets[i]->tid;
			all_tweet->tweet_text = user_tweets[i]->tweet;
			all_tweet->userid = user_tweets[i]->userid;
			//all_tweet->count_of_retweets_and_likes = user_tweets[i]->count;
			all_tweets[i] = all_tweet;

		}
	}
	*l = total_len;
	return all_tweets;
}

struct tweet **gettweets()
{
	struct tweetfile **temp = getTweets();
	struct tweet **res = (struct tweet**)malloc(sizeof(struct tweet*) * 5);
	for (int i = 0; i < 5; i++)
	{
		res[i] = (struct tweet *)malloc(sizeof(struct tweet));
		res[i]->userid = temp[i]->userid;
		res[i]->tid = temp[i]->tid;
		res[i]->tweet_text = temp[i]->tweet;
		res[i]->username = getUserDetails(temp[i]->userid)->uname;
	}
	return res;
}

int changeUserName(int userid, char* username)
{
	return changeUsername(userid, username);
}

void changePassword(int userid, char *password)
{
	changeUserPassword(userid, password);
}




//int followUser(int userid, char *name)
//{
//	return createFollow(userid, getUserId(name));
//}

int editTweet(int tid, int userid, char *tweet)//;// {}
{
	return updateTweet(tid, userid, tweet);
}

int delTweet(int tid, int userid)
{
	//return 0;
	return deleteTweet(tid, userid);
}

//int favourite(int tid, int userid)
//{
//	//return 0;
//	return addfav(tid, userid);
//}


char **myFollowers(int userid, int *len){
	//struct userfile * userprofile = (struct userfile*) malloc(sizeof(struct userfile) * (*len));
	struct followersfile **followers_file = (struct followersfile**) malloc(sizeof(struct followersfile*) * (*len));
	int i;
	char **followers = (char**)malloc(sizeof(char*) * 300);
	struct followersfile** f_file = getUserFollowers(userid, len);
	for (i = 0; i < *len; i++){
		struct userfile* user_details = getUserDetails(f_file[i]->followeruid);//followers_file[i]->followeruid);
		char *follower = (char*)malloc(30 * sizeof(char));
		follower = user_details->uname;
		followers[i] = follower;
	}

	return followers;
}

char **myFollowing(int userid, int *len){
	//struct userfile * userprofile = (struct userfile*) malloc(sizeof(struct userfile) * (*len));
	struct followersfile **followers_file = (struct followersfile**) malloc(sizeof(struct followersfile*) * (*len));
	int i;
	char **followers = (char**)malloc(sizeof(char*) * 300);
	struct followersfile** f_file = getUserFollowing(userid, len);
	for (i = 0; i < *len; i++){
		struct userfile* user_details = getUserDetails(f_file[i]->userid);
		char *follower = (char*)malloc(30 * sizeof(char));
		follower = user_details->uname;
		followers[i] = follower;
	}

	return followers;
}

void load()
{
	loadData();
}



//char **followedByMe(int userid, int *len){}
//
////int comment(int userid, int tid, char* comment){}
//
//struct user* viewProfile(char *name){}



int validate_user(char* username, char *password, char *email)
{
	int check = getUserId(username);
	if (check == -1)
	{
		createUser(username, password, email);
		return 0;
	}
	else
	{
		return 1;
	}
}

void storeUser(char);
int login(char *username, char *password)
{
	//user name checking
	int userid = getUserId(username);
	if (userid != -1)
	{

		struct userfile* user = getUserDetails(userid);
		if (strcmp(password, user->password) == 0)
		{
			return userid;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}

}
void logout()
{
	updateAll();
}
int postTweet(char *tweet, int userid)
{
	return createTweet(userid, tweet);
}
struct tweet** getMyTweets(int userid, int *lent)
{
	int i;
	struct tweetfile** tweets = getUserTweets(userid, lent);
	struct tweet **mytweets = (struct tweet**)malloc(*lent * sizeof(tweet));
	for (i = 0; i < 30; i++)
	{
		mytweets[i] = (struct tweet*)malloc(sizeof(struct tweet));
	}
	for (i = 0; i < *lent; i++)
	{
		mytweets[i]->tweet_text = tweets[i]->tweet;
		mytweets[i]->tid = tweets[i]->tid;
		mytweets[i]->userid = tweets[i]->tid;
		struct userfile *forusername = getUserDetails(userid);
		mytweets[i]->username = forusername->uname;
	}
	return mytweets;

}
struct user* displayUsersProfile(int userid)
{
	struct user* myprofile=(struct user*)malloc(sizeof(struct user)*1);
	struct userfile* userdata = getUserDetails(userid);
	myprofile->userid = userid;
	myprofile->username=userdata->uname;
	myprofile->email=userdata->email;
	/*struct followersfile **followers = (struct followersfile**)malloc(300 * sizeof(followersfile*));
	int i;
	for (i = 0; i < 300; i++)
	{
		followers[i] = (struct followersfile*)malloc(sizeof(struct followersfile));
	}*/
	int len_followers=0;

	getUserFollowers(userid, &len_followers);
	int len_following = getFollowing(userid);

	myprofile->followers = len_followers;
	myprofile->following = len_following;
	int my_tweet_count;
	getMyTweets(userid, &my_tweet_count);
	myprofile->my_tweet_count = my_tweet_count;
	return myprofile;

}

int comment(int userid, int tid, char* comment)
{
	int cid = createComment(tid, userid, comment);
	return cid;
}

struct user* viewmyprofile(char *username)
{
	int userid = getUserId(username);
	struct user* profile = displayUsersProfile(userid);
	return profile;
}
int likeTweet(int tid, int userid)
{
	int i = addfav(tid, userid);
	return i;
}
int deletetweet(int tid, int userid)
{
	deleteTweet(tid, userid);
	return 1;
}
int favourite(int tid, int userid)
{
	int len=0;
	int i, hasfavorite = 0;
	struct tweetfile** tweets = getUserTweets(userid, &len);
	struct tweet **mytweets = (struct tweet**)malloc(len * sizeof(tweet));
	for (i = 0; i < 30; i++)
	{
		mytweets[i] = (tweet*)malloc(sizeof(tweet));
	}
	for (i = 0; i < len; i++)
	{
		if (tid == tweets[i]->tid && userid == tweets[i]->userid)
		{
			hasfavorite = 1;
			return tweets[i]->count;
		}
	}
	if (hasfavorite == 0)
		return 0;
}
int followUser(int userid, char *name)
{
	int followerid = getUserId(name);
	int following = createFollow(userid, followerid);
	return following;
}



struct tweet** trendingTweets()
{
	int len = 5;
	struct tweetfile** db_tweet = getTweets();
	struct tweet** trending_tweets = (struct tweet**)malloc(len*sizeof(struct tweet*));
	for (int i = 0; i < len; i++){
		struct tweet* new_tweet = (struct tweet*)malloc(sizeof(struct tweet));
		new_tweet->tid = db_tweet[i]->tid;
		new_tweet->tweet_text = db_tweet[i]->tweet;
		new_tweet->userid = db_tweet[i]->userid;
		//	new_tweet->count_of_retweets_and_likes = db_tweet[i]->count;
		trending_tweets[i] = new_tweet;
	}
	return trending_tweets;
}



