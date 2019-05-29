# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.shortcuts import render, redirect
from django.http import HttpResponse

from asker.forms import QuestionForm, AnswerForm, RegisterForm, LoginForm,SettingsForm

from faker import Faker
from django.core.paginator import EmptyPage, PageNotAnInteger, Paginator
from django.shortcuts import render, get_object_or_404
from django.db.models import Count

from django.contrib import auth

from asker.models import Question,Profile,Tag,Answer, Like
from django.contrib.auth.models import User
from django.http import JsonResponse
from django.core.exceptions import ValidationError
from django.contrib.auth.decorators import login_required

fake = Faker()
# Create your views here.

def paginate(objects_list, request, page_size=10):
    # do smth with Paginator, etc…
    
    paginator = Paginator(objects_list, page_size)
    page = request.GET.get('page')
    #№objects_page = paginator.page()
    objects_page = paginator.get_page(page)
  
    return objects_page,paginator

def index(request):
    questions = Question.objects.new()
    questions,p = paginate(questions, request, 20)
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
    questions,p = paginate(questions, request, 20)
    questions.object_list = questions.object_list.annotate(answ_count=Count('answer'))
    
    users_top = Profile.objects.user_top()
    popular_tags = Tag.objects.popular_tags()
    context = {}
    context['questions'] = questions
    context['users_top'] = users_top
    context['popular_tags'] = popular_tags
    return render(request,'index.html',context=context)

def login(request):
    if request.POST:
        form = LoginForm(request.POST)
        if form.is_valid():
            cdata = form.cleaned_data
            user = auth.authenticate(**cdata)
            if user is not None:
                auth.login(request, user)
                return redirect(request.POST.get('next', '/'))
            form.add_error(None, 'no such user')
    else:
        form = LoginForm()
    users_top = Profile.objects.user_top()
    popular_tags = Tag.objects.popular_tags()
    return render(request,'login.html',{'users_top': users_top, 
                                        'popular_tags': popular_tags,
                                        'form':form,
                                        'next':request.GET.get('next','/')
                                       })


def question(request,qid):
    if request.POST:
        if not request.user.is_authenticated:
            return redirect('/login/') 
        form = AnswerForm(request.POST)
        if form.is_valid():
            cdata = form.cleaned_data
            Answer.objects.create(
            text=cdata['text'],
            author=request.user.profile,
            question_id = qid)
            question = get_object_or_404(Question, pk=qid)
            answers = question.answer_set.all()
            answers,p = paginate(answers, request, 30)
            
            return redirect('/question/'+str(qid)+'/'+'?page='+str(p.num_pages)) #todo правильные редиректы
    else:
        form = AnswerForm()
    question = get_object_or_404(Question, pk=qid)
    answers = question.answer_set.all()
    question.answ_count = answers.count()
    answers,p = paginate(answers, request, 30)
    answers.object_list = answers.object_list.annotate (likes=Count('like'))
    
    users_top = Profile.objects.user_top()
    popular_tags = Tag.objects.popular_tags()
    context = {}
    context['form'] = form
    context['question'] = question
    context['answers'] = answers
    context['users_top'] = users_top
    context['popular_tags'] = popular_tags
    return render(request,'question.html',context=context)

def register(request):
    #try https://docs.djangoproject.com/en/2.2/howto/custom-template-tags/
    if request.POST:
        form = RegisterForm(request.POST)
        if form.is_valid():
            cdata = form.cleaned_data
            if cdata['password'] == cdata['rep_password']:
                u = User.objects.create_user(
                     username=cdata['username'],
                     email=cdata['email'],
                     password=cdata['password'])
                Profile.objects.create(user=u)
                return redirect('/') #todo правильные редиректы
            else:
                 form.add_error('password', ValidationError(('Пароли должны совпадать!'), code='invalid'))
                 form.add_error('rep_password', ValidationError(('Пароли должны совпадать!'), code='invalid'))
    else:
        form = RegisterForm()
    
    users_top = Profile.objects.user_top()
    popular_tags = Tag.objects.popular_tags()
    return render(request, 'register.html', {'form':form,
        'users_top': users_top, 
        'popular_tags': popular_tags})

@login_required
def settings(request):
    if request.POST:
        form = SettingsForm(request.POST, request.FILES)
        if form.is_valid():
            cdata = form.cleaned_data
            Profile.objects.update(request.user, cdata)
            return redirect('/')
    else:
        form = SettingsForm()
    users_top = Profile.objects.user_top()
    popular_tags = Tag.objects.popular_tags()
    return render(request, 'settings.html',{'form':form,
        'users_top': users_top, 
        'popular_tags': popular_tags})

@login_required
def ask(request):
    if request.POST:
        form = QuestionForm(
            data=request.POST)
        if form.is_valid():
            cdata = form.cleaned_data
            q = Question.objects.create(
                title=cdata['title'],
                text=cdata['text'],
                author_id=request.user.profile.id
            )
            for tag in cdata['tags']:
                q.tags.add(tag)
            return redirect('/question/'+str(q.id)+'/')
    else:
        form = QuestionForm()
        
    users_top = Profile.objects.user_top()
    popular_tags = Tag.objects.popular_tags()
    return render(request, 'ask.html',{'form':form,
        'users_top': users_top, 
        'popular_tags': popular_tags})

def tag(request, tagname):
    tag = get_object_or_404(Tag,tagname=tagname)
    questions = tag.question_set.all().order_by('-rating')
    questions,p = paginate (questions, request, 20)
    questions.object_list = questions.object_list.annotate(answ_count=Count('answer'))
    
    users_top = Profile.objects.user_top()
    popular_tags = Tag.objects.popular_tags()
    context = {}
    context['questions'] = questions
    context['users_top'] = users_top
    context['popular_tags'] = popular_tags
    context['tagname'] = tagname
        
    return render(request,'tag.html',context=context)

@login_required
def signout(request):
    auth.logout(request)
    return redirect('/')

@login_required
def like_article(request):
    question_id = request.POST.get('question_id', '')
    question = Question.objects.filter(id=question_id).first()
    if not question:
        return JsonResponse({"status": "error"})
    like  = Like.objects.filter(author_id=request.user.id,question_id=question_id).first()
    if like:
        return JsonResponse({"status": "error"})
    Like.objects.create(author_id=request.user.id, question_id=question_id)
    question.rating += 1
    question.save()
    return JsonResponse({"status": "ok"})

@login_required
def like_answer(request):
    answer_id = request.POST.get('answer_id','')
    answer = Answer.objects.filter(id=answer_id).first()
    if not answer:
        return JsonResponse({"status": "error"})
    like  = Like.objects.filter(author_id=request.user.id,answer_id=answer_id).first()
    if like:
        return JsonResponse({"status": "error"})
    Like.objects.create(author_id=request.user.id, answer_id=answer_id)
    answer.rating +=1
    answer.save()
    return JsonResponse({"status": "ok"})

@login_required
def dislike_article(request):
    question_id = request.POST.get('question_id', '')
    question = Question.objects.filter(id=question_id).first()
    if not question:
        return JsonResponse({"status": "error"})
    like  = Like.objects.filter(author_id=request.user.id,question_id=question_id).first()
    if like:
        return JsonResponse({"status": "error"})
    Like.objects.create(author_id=request.user.id, question_id=question_id)
    question.rating -= 1
    question.save()
    return JsonResponse({"status": "ok"})

@login_required
def dislike_answer(request):
    answer_id = request.POST.get('answer_id','')
    answer = Answer.objects.filter(id=answer_id).first()
    if not answer:
        return JsonResponse({"status": "error"})
    like  = Like.objects.filter(author_id=request.user.id,answer_id=answer_id).first()
    if like:
        return JsonResponse({"status": "error"})
    Like.objects.create(author_id=request.user.id, answer_id=answer_id)
    answer.rating -=1
    answer.save()
    return JsonResponse({"status": "ok"})
    