# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.shortcuts import render
from django.http import HttpResponse

from faker import Faker

fake = Faker()
# Create your views here.

def index(request):
    questions = [
        {'id' : i, 
         'title' : fake.sentence(),
         'text' : '\n'.join(fake.sentences(fake.random_int(1,3)))
        }
        for i in range(10)
    ]
    return render(request,'questions.html',{'questions': questions})


def login(request):
    return render(request,'log.html',{})


def question(request,qid):
    return render(request,'question.html',{})