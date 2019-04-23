# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.shortcuts import render
from django.http import HttpResponse

from faker import Faker
from django.core.paginator import EmptyPage, PageNotAnInteger, Paginator
from django.shortcuts import render, get_object_or_404
from django.db.models import Count


#from asker import Question
from asker.models import Question,Profile,Tag,Answer


fake = Faker()
# Create your views here.

def paginate(objects_list, request, page_size=10):
    # do smth with Paginator, etc…
    
    paginator = Paginator(objects_list, page_size)
    page = request.GET.get('page')
    objects_page = paginator.get_page(page)
  
    return objects_page

def index(request):
    questions = Question.objects.new()
    questions = paginate(questions, request, 20)
    questions.object_list = questions.object_list.annotate(answ_count=Count('answer'))
    
    users_top = Profile.objects.user_top()
    popular_tags = Tag.objects.popular_tags()
    context = {}
    context['questions'] = questions
    context['users_top'] = users_top
    context['popular_tags'] =  popular_tags
    return render(request,'index.html',context=context)
        

def hot(request):
    questions = Question.objects.best()
    questions = paginate(questions, request, 20)
    questions.object_list = questions.object_list.annotate(answ_count=Count('answer'))
    
    users_top = Profile.objects.user_top()
    popular_tags = Tag.objects.popular_tags()
    context = {}
    context['questions'] = questions
    context['users_top'] = users_top
    context['popular_tags'] = popular_tags
    return render(request,'index.html',context=context)

def login(request):
    users_top = Profile.objects.user_top()
    popular_tags = Tag.objects.popular_tags()
    return render(request,'login.html',{'users_top': users_top, 'popular_tags': popular_tags})


def question(request,qid):
    question = get_object_or_404(Question, pk=qid)
    answers = question.answer_set.all()
    question.answ_count = answers.count()
    answers = paginate(answers, request, 30)
    answers.object_list = answers.object_list.annotate (likes=Count('like'))
    
    users_top = Profile.objects.user_top()
    popular_tags = Tag.objects.popular_tags()
    context = {}
    context['question'] = question
    context['answers'] = answers
    context['users_top'] = users_top
    context['popular_tags'] = popular_tags
    return render(request,'question.html',context=context)

def register(request):
    users_top = Profile.objects.user_top()
    popular_tags = Tag.objects.popular_tags()
    return render(request, 'register.html', {
        'users_top': users_top, 
        'popular_tags': popular_tags})

def settings(request):
    users_top = Profile.objects.user_top()
    popular_tags = Tag.objects.popular_tags()
    return render(request, 'settings.html',{
        'users_top': users_top, 
        'popular_tags': popular_tags})

def ask(request):
    users_top = Profile.objects.user_top()
    popular_tags = Tag.objects.popular_tags()
    return render(request, 'ask.html',{
        'users_top': users_top, 
        'popular_tags': popular_tags})

def tag(request, tagname):
    tag = get_object_or_404(Tag,tagname=tagname)
    questions = tag.question_set.all().order_by('-rating')
    questions = paginate (questions, request, 20)
    questions.object_list = questions.object_list.annotate(answ_count=Count('answer'))
    
    users_top = Profile.objects.user_top()
    popular_tags = Tag.objects.popular_tags()
    context = {}
    context['questions'] = questions
    context['users_top'] = users_top
    context['popular_tags'] = popular_tags
    context['tagname'] = tagname
        
    return render(request,'tag.html',context=context)