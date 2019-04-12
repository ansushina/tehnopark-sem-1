# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.shortcuts import render
from django.http import HttpResponse

from faker import Faker
from django.core.paginator import EmptyPage, PageNotAnInteger, Paginator
from django.shortcuts import render, get_object_or_404


#from asker import Question
from asker.models import Question,Profile,Tag,Answer


fake = Faker()
# Create your views here.

def paginate(objects_list, request, page_size=10):
    # do smth with Paginator, etc…
    paginator = Paginator(objects_list, page_size)
    page = request.GET.get('page')
    objects_page = paginator.get_page(page)
    prev_page = 0
    next_page = 0
    page_number = objects_page.number
    if (objects_page.has_previous):
        prev_page = objects_page.previous_page_number
    if (objects_page.has_next):
        next_page = objects_page.next_page_number
    paginator = {'page_number': page_number,
                'prev_page': prev_page,
                'next_page': next_page,
                'page': page,
                }
    return objects_page, paginator

def index(request):
    questions = Question.objects.new()
    questions, context = paginate (questions, request,20)
    users_top = Profile.objects.user_top()
    popular_tags = Tag.objects.popular_tags()
    
    context['questions'] = questions
    context['login_flag'] =  0
    context['users_top'] = users_top
    context['popular_tags'] =  popular_tags
    return render(request,'index.html',context=context)
        

def hot(request):
    #user = request.user
    questions = Question.objects.best()
    
    questions, context = paginate (questions, request,20)
    users_top = Profile.objects.user_top()
    popular_tags = Tag.objects.popular_tags()
    
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
    flag = 1;
    answers = Question.objects.answers(question)#Answer.objects.answ_for_question(question)
    answers, context = paginate (answers, request,30)
    users_top = Profile.objects.user_top()
    popular_tags = Tag.objects.popular_tags()
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
    questions = Tag.objects.questions(tag)#Question.objects.by_tag(tagname)
    questions, context = paginate (questions, request,20)
    users_top = Profile.objects.user_top()
    popular_tags = Tag.objects.popular_tags()
        
    context['questions'] = questions
    context['login_flag'] = flag
    context['users_top'] = users_top
    context['popular_tags'] = popular_tags
    context['tagname'] = tagname
        
    return render(request,'tag.html',context=context)