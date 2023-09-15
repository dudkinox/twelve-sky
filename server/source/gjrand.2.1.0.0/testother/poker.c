/* Test software for gjrand random numbers version 2.1.0.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* A test for gjrand_shuffle(). This simulates a very simplified version */
/* of the game of poker. */

/* In this simplified version, there are 10 players and a deck of 52 cards. */
/* The cards consist of 4 suits each with 13 different numbers 1 to 13. */
/* A game consists of a number of hands (specified on command line). */
/* For each hand, each player is dealt 5 cards (and 2 cards remain unused). */
/* The player with the best hand wins. There are various tie-breaks so */
/* ties never happen. I just made these up, so if you play poker, you may be */
/* used to slightly different tie-breaks. */
/* There is no betting. We just count how often each player wins. */

/* Hands that are worth something (in decreasing order): */
/* 8. Straight flush (any hand that qualifies as both straight, and flush). */
/* 7. Four of a kind (4 cards have the same number). */
/* 6. Full house (3 cards have the same number, and 2 another same number.) */
/* 5. Flush (all 5 cards are from the same suit). */
/* 4. Straight (All 5 cards have consecutive numbers. 1 counts as 14 */
/*	if it helps.) */
/* 3. Three of a kind (3 cards have the same number). */
/* 2. Two pairs (2 cards have the same number, and 2 another same number). */
/* 1. One pair (2 cards have the same number). */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../src/gjrand.h"

static void
crash(const char *s)
{
    fprintf(stderr, "crash [%s]\n", s); exit(1);
}

static int
isflush(int hand[5])
{
    int s=hand[0] & 0x3;

    return (s==(hand[1]&0x3) && s==(hand[2]&0x3)
	&& s==(hand[3]&0x3) && s==(hand[4]&0x3));
}

static void
dosort(int hand[5])
{
    int i, j, k;

    for (i=1; i<5; i++)
    {
	k=hand[i];
	for (j=i-1; j>=0 && hand[j]>k; j--) hand[j+1]=hand[j];
	hand[j+1]=k;
    }
}

/* Has the side effect of sorting the hand. This is relied on */
/* by some functions called afterwards. */
static int
isstraight(int hand[5])
{
    int i, k;

    /* Aces usually high */
    for (i=0; i<5; i++) if (hand[i]<4) hand[i]+=52;

    dosort(hand);

    /* Ace low if it helps make a straight */
    if ((hand[0]>>2)==1 && (hand[1]>>2)==2 && (hand[2]>>2)==3
	&& (hand[3]>>2)==4 && (hand[4]>>2)==13)
    {
	k=hand[4]-52;
	for (i=4; i>0; i--) hand[i]=hand[i-1];
	hand[0]=k;
    }

    for (i=1; i<5; i++) if ((hand[i]>>2)!=(hand[i-1]>>2)+1) return 0;

    return 1;
}

/* assumes sorted already by isstraight. */
/* looks for 2, 3, 4 of a kind, 2 pair, full house. */
/* re-sorts if necessary for correct tiebreak. */
static void
numbers(int hand[5], char eval[11])
{
    int st=5, le=1, i, r=0;

    /* mark each card if it is part of a pair, triple or quad */
    for (i=1; i<5; i++)
    {
	if ((hand[i]>>2)==(hand[i-1]>>2))
	{
	    if (le==1) st=i-1;
	    le++;
	}
	else if (le>1)
	{
	    while (st<i) {hand[st]|=le<<8; st++;}
	    st=5; le=1;
	}
    }
    if (le>1)
    {
	while (st<5) {hand[st]|=le<<8; st++;}
	st=5; le=1;
    }

    /* re-sort putting number duplicates at high positions. */
    /* this is what we want for tiebreak. */
    dosort(hand);

    /* nothing hand */
    le=hand[4]>>8;
    if (le == 0) goto ret;

    /* if here, there is at least a pair */
    /* check if it is not better than that */
    if (le == 2)
    {
	r=1;
	if (hand[2]>>8 == 0) goto ret; /* 1 pair */
	r=2; /* 2 pair */
	goto ret;
    }

    /* at least a triple */
    if (le == 3)
    {
	r=3;
	if (hand[1]>>8 == 0) goto ret; /* 3 of a kind */
	r=6; /* full house. */
	goto ret;
    }

    if (le == 4) r=7; /* 4 of a kind */
    else crash("numbers ?");

    ret:
    /* remove marks from cards */
    for (i=0; i<5; i++) hand[i]&=255;

    eval[0]=r;

    return;
}

static void
tiebreak(int hand[5], char eval[11])
{
    eval[1]=hand[4]>>2;
    eval[2]=hand[3]>>2;
    eval[3]=hand[2]>>2;
    eval[4]=hand[1]>>2;
    eval[5]=hand[0]>>2;
    eval[6]=hand[4]&0x3;
    eval[7]=hand[3]&0x3;
    eval[8]=hand[2]&0x3;
    eval[9]=hand[1]&0x3;
    eval[10]=hand[0]&0x3;
}

static void
classify(int hand[5], char eval[11])
{
    int f=isflush(hand), s=isstraight(hand);

    if (s) {eval[0]=4; if (f) eval[0]=8;}
    else if (f) eval[0]=5;
    else numbers(hand, eval);
    tiebreak(hand, eval);
}

static void
showhand(FILE *fp, int hand[5])
{
    int i;

    for (i=0; i<5; i++)
    {
	int c=hand[i];
	int n=c>>2, s=c&0x3;
	const char sc[]="cdhs", nc[]="A23456789TJQKA";

	fputc(nc[n], fp); fputc(sc[s], fp); fputc(' ', fp);
    }
}

static void
mkplaces(char eval[10][11], int place[10])
{
    int i, j, k;

    for (i=0; i<10; i++) place[i]=i;

    for (i=1; i<10; i++)
    {
	k=place[i];
	for (j=i-1; j>=0 && memcmp(eval[place[j]], eval[k], 11)<0; j--)
	    place[j+1]=place[j];
	place[j+1]=k;
    }
}

static void
usage(GJRAND_NO_ARGS)
{
    fprintf(stderr, "Usage:\n"
"./poker -t\n"
"./poker number-of-hands\n"
"./poker number-of-hands random-seed\n"
);
    exit(1);
}

static int64_t places[10][10];

static void
placechi(int64_t count)
{
    int i, j;
    double x, t, c;

    /* df = 9 ? */
    printf("expect [ 1.16 2.09 4.17 14.7 21.7 27.9 ]\n");
    printf("places chi-squared =\n");
    if (count<100)
	fprintf(stderr, "Warning < 100 hands, don't trust places chisquared\n");
    x=((double)count)/10.0;
    for (i=0; i<10; i++)
    {
	t=0.0;
	for (j=0; j<10; j++)
	{
	    c=places[i][j]-x;
	    t+=c*c;
	}
	t/=x;
	if (i==5) putchar('\n');
	printf("%10.5f  ", t);
    }
    putchar('\n');
}

static int64_t hands[10][10];

static void
handschi(int64_t count)
{
    int i, j;
    static const long toti=2598960;
    static const long xi[9]=
    {
	1302540,
	1098240,
	123552,
	54912,
	10200,
	5108,
	3744,
	624,
	40
    };
    double x[9], t, c;

    if (count<100000) fprintf(stderr,
"Warning < 100000 hands, don't trust hands chisquared\n");
    t=0.0;
    for (i=0; i<9; i++) x[i]=(double)(xi[i])*count/toti;
    for (i=0; i<10; i++) for (j=0; j<9; j++)
    {
	c=hands[i][j]-x[j];
	t+=c*c/x[j];
    }

    /* df = 80 ? */
    printf("expect [ 46.5 53.5 64.3 96.6 112 125 ]\n");
    printf("hands chi-squared = %10.5f\n", t);
}

int
main(int argc, char **argv)
{
    int i, j, cards[52], place[10];
    int64_t count, j64;
    double dcount;
    unsigned long seed;
    char eval[10][11];
    struct gjrand g;

    gjrand_initrand(&g);

    if (argc<2 || argc>3) usage();

    if (argc==2 && strcmp(argv[1], "-t")==0)
    {
	for (j=0; j<3; j++)
	{
	    gjrand_shuffle(&g, 52, cards);

	    for (i=0; i<10; i++)
	    {
		printf("%1d: ", i);
		showhand(stdout, cards+i*5);
		classify(cards+i*5, eval[i]);
		printf(":%1d: ", eval[i][0]);
		showhand(stdout, cards+i*5);
		putchar('\n');
	    }

	    mkplaces(eval, place);
	    for (i=0; i<10; i++) printf("%2d", place[i]);

	    printf("\n=====\n\n");
	}
	exit(0);
    }

    if (sscanf(argv[1], "%lf", &dcount)!=1) crash("arg1 not a number");
    count = (int64_t)floor(dcount);
    if (count<1 || count!=floor(dcount)) crash("arg1 count out of range");
    if (argc>2)
    {
	if (sscanf(argv[2], "%lu", &seed)!=1) crash("arg2 not a number");
	gjrand_init(&g, seed);
    }

    memset(places, 0, sizeof(places));
    memset(hands, 0, sizeof(hands));

    for (j64=0; j64<count; j64++)
    {
	gjrand_shuffle(&g, 52, cards);
	for (i=0; i<10; i++) classify(cards+i*5, eval[i]);
	mkplaces(eval, place);
	for (i=0; i<10; i++)
	{
	    places[i][place[i]]++;
	    hands[i][(int)(eval[i][0])]++;
	}
    }

    printf("places\n");
    printf(
"      1st    2nd    3rd    4th    5th    6th    7th    8th    9th   10th\n");
    for (j=0; j<10; j++)
    {
	printf("%1d:", j);
	for (i=0; i<10; i++) printf(" %6.0f", (double)(places[j][i]));
	putchar('\n');
    }
    placechi(count);

    printf("\nhands\n");
    printf(
"   nothing       2     2+2       3 straight  flush     3+2       4   st+fl\n");
    for (j=0; j<10; j++)
    {
	printf("%1d:", j);
	for (i=0; i<9; i++) printf(" %7.0f", (double)(hands[j][i]));
	putchar('\n');
    }
    handschi(count);

    return 0;
}
