# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.shortcuts import render
from django.http import HttpResponse

from faker import Faker
from django.core.paginator import EmptyPage, PageNotAnInteger, Paginator
from django.shortcuts import render


fake = Faker()
# Create your views here.

def paginate(objects_list, request):
    # do smth with Paginator, etc…
    paginator = Paginator(objects_list, 10)
    page = request.GET.get('page')
    objects_page = paginator.get_page(page)
    return objects_page, paginator

def index(request):
    questions = Questions.object.new()
    
    flag = 0
    questions = [
        {'id' : i, 
         'title' : fake.sentence(),
         'tags' : [ fake.word() for i in range (3)],
         'text' : '\n'.join(fake.sentences(fake.random_int(1,3))), 
         'answ_count' : i,
         'likes': i
        }
        for i in range(100)
    ]
    questions, paginator = paginate (questions, request)
    users_top = [
        {'id' : i,
         'nickname' : fake.word()
        }
        for i in range(5)
    ]
    popular_tags = [
        {'id' : i,
         'tagname' : fake.word()
        }
        for i in range(10)
    ]
    
    
    return render(request,'index.html',{'questions': questions, 'login_flag' : flag, 'users_top': users_top, 'popular_tags': popular_tags})

def hot(request):
    user = request.user
    flag = 0
    questions = [
        {'id' : i, 
         'title' : fake.sentence(),
         'tags' : [ fake.word() for i in range (3)],
         'text' : '\n'.join(fake.sentences(fake.random_int(1,3))), 
         'answ_count' : i,
         'likes': i
        }
        for i in range(100)
    ]
    questions, paginator = paginate (questions, request)
    users_top = [
        {'id' : i,
         'nickname' : fake.word()
        }
        for i in range(5)
    ]
    popular_tags = [
        {'id' : i,
         'tagname' : fake.word()
        }
        for i in range(10)
    ]
    
    return render(request,'index.html',{'questions': questions, 'login_flag' : flag, 'users_top': users_top, 'popular_tags': popular_tags})

def login(request):
    flag = 0;
    users_top = [
        {'id' : i,
         'nickname' : fake.word()
        }
        for i in range(5)
    ]
    popular_tags = [
        {'id' : i,
         'tagname' : fake.word()
        }
        for i in range(10)
    ]
    return render(request,'login.html',{'login_flag':flag, 'users_top': users_top, 'popular_tags': popular_tags})


def question(request,qid):
    flag = 1;
    question = {'id' : qid, 
         'title' : fake.sentence(),
         'tags' : [ fake.word() for i in range (3)],
         'text' : '\n'.join(fake.sentences(fake.random_int(5,20))), 
         'answ_count' : qid,
         'likes': qid}
    answers = [
         {'id' : i,
          'likes': i,
         'text' : '\n'.join(fake.sentences(fake.random_int(1,3)))
        }
        for i in range(int(qid))
    ]
    users_top = [
        {'id' : i,
         'nickname' : fake.word()
        }
        for i in range(5)
    ]
    popular_tags = [
        {'id' : i,
         'tagname' : fake.word()
        }
        for i in range(10)
    ]
    return render(request,'question.html',{'question':question, 'answers' : answers, 'login_flag':flag, 'users_top': users_top, 'popular_tags': popular_tags})

def register(request):
    flag = 0;
    users_top = [
        {'id' : i,
         'nickname' : fake.word()
        }
        for i in range(5)
    ]
    popular_tags = [
        {'id' : i,
         'tagname' : fake.word()
        }
        for i in range(10)
    ]
    return render(request, 'register.html', {'login_flag':flag, 'users_top': users_top, 'popular_tags': popular_tags})

def settings(request):
    flag = 1;
    users_top = [
        {'id' : i,
         'nickname' : fake.word()
        }
        for i in range(5)
    ]
    popular_tags = [
        {'id' : i,
         'tagname' : fake.word()
        }
        for i in range(10)
    ]
    return render(request, 'settings.html',{'login_flag':flag, 'users_top': users_top, 'popular_tags': popular_tags})

def ask(request):
    flag = 1;
    users_top = [
        {'id' : i,
         'nickname' : fake.word()
        }
        for i in range(5)
    ]
    popular_tags = [
        {'id' : i,
         'tagname' : fake.word()
        }
        for i in range(10)
    ]
    return render(request, 'ask.html',{'login_flag':flag, 'users_top': users_top, 'popular_tags': popular_tags})

def tag(request, tagname):
    flag = 1;
    questions = [
        {'id' : i, 
         'title' : fake.sentence(),
         'tags' : [fake.word() for i in range (2)],
         'text' : '\n'.join(fake.sentences(fake.random_int(1,3))), 
         'answ_count' : i,
         'likes': i
        }
        for i in range(100)
    ]
    questions, paginator = paginate (questions, request)
    users_top = [
        {'id' : i,
         'nickname' : fake.word()
        }
        for i in range(5)
    ]
    popular_tags = [
        {'id' : i,
         'tagname' : fake.word()
        }
        for i in range(10)
    ]
    for question in questions:
        question['tags'].append(tagname);
        
    return render(request,'tag.html',{'questions': questions, 'login_flag' : flag, 'tagname' : tagname, 'users_top': users_top, 'popular_tags': popular_tags})