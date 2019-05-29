"""askme URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/1.11/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  url(r'^$', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  url(r'^$', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.conf.urls import url, include
    2. Add a URL to urlpatterns:  url(r'^blog/', include('blog.urls'))
"""
from django.conf.urls import url
from django.contrib import admin

from asker import views
from django.conf import settings

from django.conf.urls.static import static

urlpatterns = [
    url(r'^admin/', admin.site.urls),
    url('login/', views.login, name = 'login'),
    url('signup/', views.register, name = 'register'),
    url('settings/', views.settings, name = 'settings'),
    url('ask/', views.ask, name = 'ask'),
    url(r'^question/(?P<qid>\d+)/$', views.question ,name = 'question'),
    url(r'^tag/(?P<tagname>\w+)/$', views.tag ,name = 'tag'),
    url('signout/', views.signout, name='signout'),
    url('like_article/', views.like_article, name='like_article'),
    url('like_answer/', views.like_answer, name='like_answer'),
    url('dislike_article/', views.dislike_article, name='dislike_article'),
    url('dislike_answer/', views.dislike_answer, name='dislike_answer'),
    url('hot/', views.hot, name = 'hot'),
    url('', views.index, name = 'index'),   
] + static(settings.MEDIA_URL, document_root=settings.MEDIA_ROOT)
