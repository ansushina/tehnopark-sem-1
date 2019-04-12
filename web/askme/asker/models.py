from django.db import models
from django.conf import settings

# Create your models here.

class ProfileManager(models.Manager):
    def user_top(self):
        return self.order_by('-rating')[:5]

class Profile(models.Model):
    user = models.OneToOneField(
        settings.AUTH_USER_MODEL,
        on_delete=models.CASCADE)
    #avatar
    rating = models.IntegerField(default=0)
    
    objects = ProfileManager()

    def __str__(self):
        return self.user.username


class QuestionManager(models.Manager):

    def new(self):
        return self.order_by('-created_at')

    def best(self):
        return self.order_by('-rating')
    
    def answers(self,question):
        return question.answer_set.all()
    
class AnswerManager(models.Manager):
    def answ_for_question(self,question):
        all_answ = self.all()
        q_answ = []
        for answ in all_answ:
            if answ.question == question:
                q_answ.append(answ)
        return q_answ
    def create_answer(self,author, question, text):
        answer = self.create(
            author_id = author,
            question_id = question, 
            text = text,
        )
        question = Question.objects.get(pk=question)
        question.answ_count += 1
        question.save(update_fields=['answ_count'])
            
    
class TagManager(models.Manager):
    def questions(self, tag):
        return tag.question_set.all().order_by('-rating')
    def popular_tags(self):
        return self.order_by('-count')[:20]
    def plus_to_count(self, tag_id):
        tag = Tag.objects.get(pk=tag_id)
        tag.count += 1
        tag.save(update_fields=['count'])
        

    
class Tag(models.Model):
    tagname = models.CharField(max_length=32)
    count = models.IntegerField(default=0)
    objects = TagManager()
    def __str__(self):
         return self.tagname
    
class Question(models.Model):

    objects = QuestionManager()

    author = models.ForeignKey(
        to=Profile, on_delete=models.CASCADE
    )
    title = models.CharField(max_length=128)
    text = models.TextField()
    created_at = models.DateTimeField(
        auto_now_add=True
    )
    rating = models.IntegerField(default=0)
    answ_count = models.IntegerField(default=0)
    tags = models.ManyToManyField(to=Tag)
    #answers

    def __str__(self):
        return str(self.pk)+str(self.title)

class Answer(models.Model):
    author = models.ForeignKey(
        to=Profile, on_delete=models.CASCADE
    )
    question = models.ForeignKey(
        to=Question, on_delete=models.CASCADE
    )
    rating = models.IntegerField(default=0)
    text = models.TextField()
    created_at = models.DateTimeField(
        auto_now_add=True
    )
    objects = AnswerManager()
    def __str__(self):
        return self.question.title
    
class Like(models.Model):
    author =  models.ForeignKey(
        to=Profile, on_delete=models.CASCADE
    )
    question = models.ForeignKey(
        to=Question, on_delete=models.CASCADE
    )
    answer = models.ForeignKey(
        to=Answer, on_delete=models.CASCADE
    )
 