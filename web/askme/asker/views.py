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
    user = request.user
    questions = Question.objects.new()
   
    questions = paginate (questions, request,20)
    questions = questions.object_list.annotate(answ_count=Count('answer'))
    users_top = Profile.objects.user_top()
    #pubs = Publisher.objects.annotate(num_books=Count('book')).order_by('-num_books')[:5]
    popular_tags = Tag.objects.annotate(count=Count('question')).order_by('-count')[:20]
    context = {}
    context['questions'] = questions
    context['login_flag'] = user.is_authenticated
    context['users_top'] = users_top
    context['popular_tags'] =  popular_tags
    return render(request,'index.html',context=context)
        

def hot(request):
    #user = request.user
    questions = Question.objects.best()
    
    questions = paginate (questions, request,20)
    questions = questions.object_list.annotate(answ_count=Count('answer'))
    users_top = Profile.objects.user_top()
    popular_tags = Tag.objects.annotate(count=Count('question')).order_by('-count')[:20]
    context = {}
    context['questions'] = questions
    context['login_flag'] = 0
    context['users_top'] = users_top
    context['popular_tags'] = popular_tags
    
    
    return render(request,'index.html',context=context)

def login(request):
    flag = 0;
    users_top = Profile.objects.user_top()
    popular_tags = Tag.objects.popular_tags()
    return render(request,'login.html',{'login_flag':flag, 'users_top': users_top, 'popular_tags': popular_tags})


def question(request,qid):
    question = get_object_or_404(Question, pk=qid)
    #question = question.annotate(answ_count=Count('answer'))
    answers = question.answer_set.all().annotate(likes=Count('like'))
    question.answ_count = answers.count()
    answers = paginate (answers, request,30)
    users_top = Profile.objects.user_top()
    popular_tags = Tag.objects.annotate(count=Count('question')).order_by('-count')[:20]
    context = {}
    context['question'] = question
    context['answers'] = answers
    context['login_flag'] = 0
    context['users_top'] = users_top
    context['popular_tags'] = popular_tags
    return render(request,'question.html',context=context)

def register(request):
    flag = 0;
    users_top = Profile.objects.user_top()
    popular_tags = Tag.objects.popular_tags()
    return render(request, 'register.html', {
        'login_flag':flag, 
        'users_top': users_top, 
        'popular_tags': popular_tags})

def settings(request):
    flag = 1;
    users_top = Profile.objects.user_top()
    popular_tags = Tag.objects.popular_tags()
    return render(request, 'settings.html',{
        'login_flag':flag, 
        'users_top': users_top, 
        'popular_tags': popular_tags})

def ask(request):
    flag = 1;
    users_top = Profile.objects.user_top()
    popular_tags = Tag.objects.popular_tags()
    return render(request, 'ask.html',{
        'login_flag':flag, 
        'users_top': users_top, 
        'popular_tags': popular_tags})

def tag(request, tagname):
    flag = 1;
    
    tag = get_object_or_404(Tag,tagname=tagname)
    questions = Tag.objects.questions(tag)
   
    questions = paginate (questions, request,20)
    questions = questions.object_list.annotate(answ_count=Count('answer'))
    users_top = Profile.objects.user_top()
    popular_tags = Tag.objects.popular_tags()
    
    context = {}
    context['questions'] = questions
    context['login_flag'] = flag
    context['users_top'] = users_top
    context['popular_tags'] = popular_tags
    context['tagname'] = tagname
        
    return render(request,'tag.html',context=context)