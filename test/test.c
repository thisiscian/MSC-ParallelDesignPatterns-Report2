#include <actormetaphor.h>
#include <mpi.h>
#include <stdio.h>

enum
{
	RETIRE,
	CHALLENGE,
	COMPLIMENT,
	CHILLING,
	START_FIGHT
};

typedef struct{
	char* name;
	int great_films;
	double bees;
} Nick_Cage;

void rehearse_being_nick_cage(Actor* actor);
void be_nick_cage(Actor* actor);
Role nick_cage = {rehearse_being_nick_cage, be_nick_cage, sizeof(Nick_Cage)};

typedef struct{
	char* nickname;
	int director;
	int actor;
} Sylvester_Stallone;

void rehearse_being_sylvester_stallone(Actor* actor);
void be_sylvester_stallone(Actor* actor);
Role sylvester_stallone = {rehearse_being_sylvester_stallone, be_sylvester_stallone, sizeof(Sylvester_Stallone)}; 

Role choose_role(int id);

int main()
{
	Actor *lead_actor = actor_initialise_metaphor(choose_role);
	MPI_Barrier(MPI_COMM_WORLD);
  perform(lead_actor); 
  actor_finalise_metaphor();
  return 0;
}

Role choose_role(int id)
{
	if(id%2 == 1)
	{
		return nick_cage;
	}
	else
	{
		return sylvester_stallone;
	}
}

void rehearse_being_nick_cage(Actor* actor)
{
	Nick_Cage* nick = actor->props;
	nick->name = "Nick Cage";
	nick->great_films = 0;
	nick->bees = 12.34;
	actor->act_number = CHILLING;
}

void be_nick_cage(Actor* actor)
{
	Nick_Cage* nick = actor->props;
	if(actor->act_number == CHILLING && actor->id >= number_of_processes)	
	{
		printf("Nick Cage(%d): I AM BORN!\n", actor->id);
	}
	else if(actor->act_number == RETIRE)
	{
		if(actor->id == 1)
		{
			get_props(actor, 1, MPI_INT, &(nick->great_films));
		}
		enter_dialogue_with_all_proteges(actor, RETIRE);
		actor->retire = 1;
		printf("Nick Cage(%d): NOT THE %d BEES (%f)\n", actor->id, nick->great_films, nick->bees);
	}
	else if (actor->act_number == CHALLENGE)
	{
		enter_dialogue(actor, 0, CHALLENGE);
		printf("Nick Cage(%d): uhhhhhhhhhhh Yeah?\n", actor->id);
		actor->act_number = CHILLING;
	}
	else if (actor->act_number == COMPLIMENT)
	{
		printf("Nick Cage(%d): oh baby\n", actor->id);
		enter_dialogue(actor, 0, COMPLIMENT);
		Actor* baby_nick = actor_train_protege(actor, nick_cage);
		actor->act_number = CHILLING;
	}
	else if (actor->act_number == CHILLING)
	{
		nick->bees += 1.0;
	}
}

void rehearse_being_sylvester_stallone(Actor* actor)
{
	Sylvester_Stallone* sly = actor->props;
	sly->nickname = "sly";
	sly->director = 0;
	sly->actor = 0;
	actor->act_number = START_FIGHT;
}

void be_sylvester_stallone(Actor* actor)
{
	Sylvester_Stallone* sly = actor->props;
	if(actor->act_number == RETIRE)
	{
		enter_dialogue_with_all_proteges(actor, RETIRE);
		actor->retire = 1;
	}
	if(actor->act_number == START_FIGHT)
	{
		if(number_of_processes == 1)
		{
			actor_train_protege(actor, nick_cage);
		}
		printf("Sylvester Stallone: you look dumb, nick\n");
		enter_dialogue(actor, 1, CHALLENGE);
		actor->act_number = CHILLING;
	}
	else if(actor->act_number == CHALLENGE)
	{
		printf("Sylvester Stallone: I take that back, you are hot stuff\n");
		enter_dialogue(actor, 1, COMPLIMENT);
		actor->act_number = CHILLING;
	}
	else if(actor->act_number == COMPLIMENT)
	{
		enter_dialogue(actor, 1, RETIRE);
		give_props(actor, 1, 1, MPI_INT, &(sly->director));
		enter_dialogue_with_all_proteges(actor, RETIRE);
		actor->retire = 1;
		printf("Sylvester Stallone: Let's get out of here together, bring your bees, I can direct the %d films\n", sly->director);
	}
	else
	{
		sly->director += 1;
	}
}
