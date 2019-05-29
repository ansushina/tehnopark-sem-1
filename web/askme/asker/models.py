from django.contrib.auth.models import User
from django.db import models
from django.conf import settings
from django.db.models import Count

# Create your models here.

class ProfileManager(models.Manager):
    def user_top(self):
        q = Question.objects.best()[:10]
        users = []
        for quest in q:
            author = quest.author
            if not author in users:
                users.append(quest.author)
        return users
    def update(self, user, cdata):
        user_fields, profile_fields = ['username', 'first_name', 'email'], ['bio', 'avatar']
        fields_to_update = {'user': [], 'profile': []}
        profile = Profile.objects.get(user=user.id)
        user = User.objects.get(pk=user.id)
        for key in user_fields:
            value = cdata.get(key, False)
            if value:
                fields_to_update['user'].append(key)
                setattr(user, key, value)


        value = cdata.get('avatar', False)
        if value:
            fields_to_update['profile'].append('avatar')
            #profile.avatar=value
        user.save(update_fields=fields_to_update['user'])
        profile.save(update_fields=fields_to_update['profile'])
        

class Profile(models.Model):
    user = models.OneToOneField(
        settings.AUTH_USER_MODEL,
        on_delete=models.CASCADE)
    avatar = models.ImageField(
        upload_to='uploads/%Y/%m/%d/', null=True, 
        max_length=100,
        blank=True
    )
    objects = ProfileManager()

    def __str__(self):
        return self.user.username
    
class TagManager(models.Manager):
    def popular_tags(self):
        return self.annotate(count=Count('question')).order_by('-count')[:20]
    
class Tag(models.Model):
    tagname = models.CharField(max_length=32)
    objects = TagManager()
    def __str__(self):
         return self.tagname


class QuestionManager(models.Manager):
    def new(self):
        return self.order_by('-created_at')

    def best(self):
        return self.order_by('-rating')     
        
class Question(models.Model):

    objects = QuestionManager()

    author = models.ForeignKey(
        to=Profile, 
        on_delete=models.CASCADE
    )
    title = models.CharField(max_length=128)
    text = models.TextField()
    created_at = models.DateTimeField(auto_now_add=True)
    rating = models.IntegerField(default=0)
    tags = models.ManyToManyField(to=Tag, null=True,  blank=True)

    def __str__(self):
        return str(self.pk)+str(self.title)
        
class Answer(models.Model):
    author = models.ForeignKey(
        to=Profile, 
        on_delete=models.CASCADE
    )
    question = models.ForeignKey(
        to=Question, 
        on_delete=models.CASCADE
    )
    rating = models.IntegerField(default=0)
    text = models.TextField()
    created_at = models.DateTimeField(auto_now_add=True)
    correct_flag = models.BooleanField(default=False)
    def __str__(self):
        return self.question.title
    
class Like(models.Model):
    author =  models.ForeignKey(
        to=Profile, 
        on_delete=models.CASCADE
    )
    question = models.ForeignKey(
        to=Question, 
        on_delete=models.CASCADE,
        null=True
    )
    answer = models.ForeignKey(
        to=Answer, 
        on_delete=models.CASCADE,  
        null=True
    )
 