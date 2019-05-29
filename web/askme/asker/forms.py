from django import forms
from django.contrib import auth

from asker.models import Profile, Question, Answer

class LoginForm(forms.Form):
    username = forms.CharField(label='Username')
    password = forms.CharField(widget=forms.PasswordInput)

class RegisterForm(forms.Form):
    username = forms.CharField()
    email = forms.EmailField()
    password = forms.CharField(widget=forms.PasswordInput)
    rep_password = forms.CharField(label='Repeat password', widget=forms.PasswordInput)
    
class QuestionForm(forms.ModelForm):  
    class Meta:
        model = Question
        fields = ['title', 'text', 'tags']
    
class AnswerForm(forms.ModelForm):
    class Meta:
        model = Answer
        fields = ['text']

class SettingsForm(forms.Form):
    username = forms.CharField(required=False)
    email = forms.EmailField(required=False)
    avatar = forms.ImageField(required=False)