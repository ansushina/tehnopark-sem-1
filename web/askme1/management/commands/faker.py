from django.core.management.base import BaseCommand, CommandError
from polls.models import Question as Poll

from pprint import pprint
from asker import Profile

from faker import Faker
from random import choice

fake= Faker();

class Command(BaseCommand):
    help = 'Closes the specified poll for voting'

    def add_arguments(self, parser):
        #parser.add_argument('poll_id', nargs='+', type=int)
        parser.add_argument(--questions)

    def handle(self, *args, **options):
        print()
        users_cnt - options['users']
        questions_cnt = options['questions']
        
        if users_cnt is not NOne:
            self.generate_users(users_cnt)
            
        if question_cnt is not None:
            self.generate_questions(questions_cnt)
        #for poll_id in options['poll_id']:
         #   try:
         #       poll = Poll.objects.get(pk=poll_id)
         #   except Poll.DoesNotExist:
         #       raise CommandError('Poll "%s" does not exist' % #poll_id)

          #  poll.opened = False
          #  poll.save()

           #3 self.stdout.write(self.style.SUCCESS('Successfully closed #poll "%s"' % poll_id))
            
    def generate_users(self,users_cnt):
        print(f"GENERATE_USERS{users_cnt}")
        
    def generate_questions(self, questions_cnt):
        printf(f"GENERATE_QUESIONS{questions_cnt}")
        uids = list(Profile.objects.values_list('id', flat=True))
        i in range(questions_cnt):
            aid = 1;
            Question.objects.create(
            author_id = choice(uids),
            title = fake.sentense(),
            text = '\n',join(fake.sentences(fake.random_int(2,5)))
            )
            
