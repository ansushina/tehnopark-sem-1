# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.conf import settings
from django.db import models
from django.contrib.auth.models import User

# Create your models here.

class Profile(models.Model):
    user = models.OneToOneField(
        settings.AUTH_USER_MODEL,
        on_delete=models.CASCADE,
    )
    #avatar
    reting = models.IntegerField(default = 0)
    def __str__(self):
        return self.user.username
    
class QuestionManager(models.Model):
    def new(self):
        return self.order_by('-created_at')
    def best(self):
        return self.order_by('rating')
    
class Question(models.Model):
    author = models.ForeignKey(
        to=Profile,
        on_delete = models.CASCADE,
    )
    title = models.CharField(max_length=128)
    text = models.TextField()
    created_at = models.DateTimeField(auto_now_add=True,)
    rating = models.IntegerField(default = 0) 
        def __str__(self):
        return f"{self.id}{self.title}"
    