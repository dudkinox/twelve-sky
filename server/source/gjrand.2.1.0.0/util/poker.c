/* Miscellaneous stuff for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* This calculates some probabilities used in testother/poker. */

/* It is implemented by taking a slightly older version of testother/poker.c */
/* and doing a fairly minimal edit, so there is probably a lot of stuff */
/* that doesn't make sense in this context. Handle with care. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../src/gjrand.h"

void
crash(char *s)
{
    fprintf(stderr, "crash [%s]\n", s); exit(1);
}

int
isflush(int hand[5])
{
    int s=hand[0] & 0x3;

    return (s==(hand[1]&0x3) && s==(hand[2]&0x3)
	&& s==(hand[3]&0x3) && s==(hand[4]&0x3));
}

void
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
int
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
void
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

void
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

void
classify(int hand[5], char eval[11])
{
    int f=isflush(hand), s=isstraight(hand);

    if (s) {eval[0]=4; if (f) eval[0]=8;}
    else if (f) eval[0]=5;
    else numbers(hand, eval);
}

void
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

long hands[9]={0,0,0,0,0,0,0,0,0};

int
main(int argc, char **argv)
{
    int i, cards[5], a, b, c, d, e;
    long count=0;
    char eval[11];

    for (a=0; a<48; a++)
    for (b=a+1; b<49; b++)
    for (c=b+1; c<50; c++)
    for (d=c+1; d<51; d++)
    for (e=d+1; e<52; e++)
    {
	cards[0]=a; cards[1]=b; cards[2]=c; cards[3]=d; cards[4]=e;
	classify(cards, eval);
	hands[(int)(eval[0])]++;
	count++;
    }

    printf("static const long count=%ld;\n", count);

    printf("static const long xi[9]={\n");
    for (i=0; i<8; i++) printf("%ld,\n", hands[i]);
    printf("%ld };\n", hands[8]);

    return 0;
}
