# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.contrib import admin

# Register your models here.

from asker.models import Profile, Question

admin.site.register(Profile)
admin.site.register(Question)