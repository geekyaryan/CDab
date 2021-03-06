#include<stdio.h>
#include<stdlib.h>
#include"lexl.c"
#include<ctype.h>
#define error printf("invalid input\n");
#define success printf("valid\n");

void program();
void declarations();
void datatype();
void identifier_list();
void statement_list();
void statement();
void assign_stat();
void expn();
void eprime();
void simple_expn();
void seprime();
void term();
void tprime();
void factor();
void relop();
void addop();
void mulop();
void decision_stat();
void looping_stat();
void dprime();

nodeptr lookahead;
FILE *fp;
int global=0;

void Error(){
	printf("Error at %s line no:%d\n",lookahead->tok.lex,lookahead->tok.row);
	//global=1;
	lookahead=lookahead->next;
	/*if(!strcmp(lookahead->tok.lex,","))
	{
		printf("Missing %s at line no:%d\n",lookahead->tok.lex,lookahead->tok.row);
		while((strcmp(lookahead->tok.lex,";"))&&(strcmp(lookahead->tok.lex,"}")))
			lookahead=lookahead->next;
	}
	else if(!strcmp(lookahead->tok.lex,";"))
	{
		printf("Missing %s at line no:%d\n",lookahead->tok.lex,lookahead->tok.row);
		while((strcmp(lookahead->tok.lex,";"))&&(strcmp(lookahead->tok.lex,"}")))
			lookahead=lookahead->next;
	}
	else
	{
		printf("Error\n");
		lookahead=lookahead->next;
	}	
	*/
}

int  match(char str[]){
	if(strcmp(lookahead->tok.lex,str) == 0)
	{
		lookahead=lookahead->next;
		return 1;
	}
	else
	{
		//Error();
		return 0;
	} 
		

}


int main()
{
	FILE *fp=fopen("input.txt","r");
	FILE *fp1=fopen("out.txt","w");
	token t1;
	nodeptr head=NULL;
	ca=getc(fp);
	while(1)
	{
		if(ca==EOF)
		{
			//display(head);
			strcpy(t1.lex,"EOF");
			strcpy(t1.type,"EOF");
			t1.col=-1;
			t1.row=-1;
  
			head=addnode(head,t1);
			//display(head);

			lookahead=head;

			
			program();

			/*if((!strcmp(lookahead->tok.lex,"EOF"))&&(global==0))
			{
				success;
			}
			else 
				error;
			*/
			printf("Parsing finished\n");
			
			return 0;
		}
		else if(ca=='\n')
		{
			linec++;
			colc=1;
		}
		else if(ca==' ')
		{
			colc++;
		}
		else if(ca=='#')
		{
			while(ca!='\n')
			{
				ca=getc(fp);
			}

			linec++;
			colc=1;
		}
		/*else if(ca=='/')
		{
			ca=getc(fp);
			if(ca=='/')
			{
				ca=getc(fp);
				while(ca!='\n')
				{
					ca=getc(fp);
				}
				linec++;
				colc=1;
				//ca=getc(fp);
			}
			
			else
			{
				ungetc(ca,fp);//ca=getc(fp);
				
			}
		}*/
		else if(ca=='\t')
		{
			colc=colc+4;
		}
		else
		{
			//printf("\n%c\n",ca);
			t1=getNextToken(fp);
			//printf("<%s,%d,%d,%s>\n",t1.lex,t1.row,t1.col,t1.type);
			fprintf(fp1,"%s\n",t1.type);
			head=addnode(head,t1);

		}
		
		ca=getc(fp);
		
		
	}
	
}



void program(){
	if(match("main"))
	{
		if(match("("))
		{
			if(match(")"))
			{
				if(match("{"))
				{
					declarations();
					statement_list();
					if(match("}"))
					{
						;
					}
					else
						Error();
				}
				else
					Error();
			}
			else
				Error();
		}
		else
			Error();
	}
	else
		Error();

		
	
	
	
	
	//match("}");
}

void declarations(){
	if(!strcmp(lookahead->tok.type,"Keyword")){
		datatype();
		identifier_list();
		if(match(";"))
			declarations();
		else
			Error();
	} else return;
}

void datatype(){
	if(strcmp(lookahead->tok.lex,"int") == 0 
		|| strcmp(lookahead->tok.lex,"char") == 0 
		|| strcmp(lookahead->tok.lex,"float") == 0){
		lookahead=lookahead->next;
	} else {
		Error();
	}
}

void identifier_list(){
	if(!strcmp(lookahead->tok.type,"Identifier")){
		lookahead=lookahead->next;
	} else {
		Error();
	}

	if(!strcmp(lookahead->tok.lex,",")){
		match(",");
		identifier_list();
	} else if(!strcmp(lookahead->tok.lex,"[")){
		if(match("["))
		{
			if(!strcmp(lookahead->tok.type,"Number"))
				lookahead=lookahead->next;
			else 
				Error();
			if(match("]"))
			{
				if(!strcmp(lookahead->tok.type,",")){
					if(match(","))
						identifier_list();
					else
						Error();
				}
			}
			else
				Error();

			
		}
		else
			Error();
		
	}
}

void statement_list(){
	if((!strcmp(lookahead->tok.type,"Identifier"))||(!strcmp(lookahead->tok.type,"Keyword")))
	{
		statement();
		statement_list();
	} else return;
}

void statement(){
	if(!strcmp(lookahead->tok.type,"Identifier"))
	{
		assign_stat();
		if(match(";"))
			;
		else
			Error();
	}
	else if(!strcmp(lookahead->tok.lex,"if"))
		decision_stat();
	else if(!strcmp(lookahead->tok.type,"Keyword"))
		looping_stat();
	else 
		Error();
	
}

void assign_stat(){
	if(!strcmp(lookahead->tok.type,"Identifier"))
		lookahead=lookahead->next;
	else
		Error();
	match("=");
	expn();
}

void expn(){ 
	simple_expn();
	eprime();
}

void eprime(){
	if(!strcmp(lookahead->tok.type,"Relational")){
		relop();
		simple_expn();
	} else return;
}

void simple_expn(){
	term();
	seprime();
}

void seprime(){
	if((!strcmp(lookahead->tok.lex,"+"))||(!strcmp(lookahead->tok.lex,"-"))){
		addop();
		term();
		seprime();
	} else return;
}

void term(){
	factor();
	tprime();
}

void tprime(){
	if((!strcmp(lookahead->tok.lex,"*"))||(!strcmp(lookahead->tok.lex,"/"))||(!strcmp(lookahead->tok.lex,"%"))){

		mulop();
		factor();
		tprime();
	} else return;
}

void factor(){
	if(!strcmp(lookahead->tok.type,"Identifier") || !strcmp(lookahead->tok.type,"Number"))
		lookahead=lookahead->next;
	else 
		Error();
}

void relop(){
	if(!strcmp(lookahead->tok.type,"Relational"))
		lookahead=lookahead->next;
	else
		Error();
}

void addop(){
	if((!strcmp(lookahead->tok.lex,"+"))||(!strcmp(lookahead->tok.lex,"-")))
		lookahead=lookahead->next;
	else 
		Error();
}

void mulop(){
	if((!strcmp(lookahead->tok.lex,"*"))||(!strcmp(lookahead->tok.lex,"/"))||(!strcmp(lookahead->tok.lex,"%")))
		lookahead=lookahead->next;
	else 
		Error();
}

void decision_stat()
{
	if(match("if"))
	{
		if(match("("))
		{
			expn();
			if(match(")"))
			{
				if(match("{"))
				{
					statement_list();
					if(match("}"))
						dprime();
					else
						Error();
				}
				else
					Error();
			}
			else
				Error();
		}
		else
			Error();
	}
	else
		Error();
	
	
	
	
	
	
	
}

void dprime()
{
	if(!strcmp(lookahead->tok.type,"Keyword")){
		if(match("else"))
		{
			if(match("{"))
			{
				statement_list();
				if(match("}"))
					;
				else 
					Error();
			}
			else
				Error();
			
		}
		else
			Error();
		
	}
	else
		return;
}

void looping_stat()
{
	if(!strcmp(lookahead->tok.lex,"while")){
		if(match("while"))
		{
			if(match("("))
			{
				expn();
				if(match(")"))
				{
					if(match("{"))
					{
						statement_list();
						if(match("}"))
							;
						else
							Error();
					}
					else
						Error();
					
				}
				else
					Error();

			}
			else
				Error();
			
		}
		else
			Error();
		
	}
	else if(!strcmp(lookahead->tok.lex,"for"))
	{
		if(match("for"))
		{
			if(match("("))
			{
				assign_stat();
				if(match(";"))
				{
					expn();
					if(match(";"))
					{
						assign_stat();
						if(match(")"))
						{
							if(match("{"))
							{
								statement_list();
								if(match("}"))
									;
								else
									Error();
							}
							else
								Error();
							
						}
						else
							Error();
					
					}
					else
						Error();
					

				}
				else
					Error();
				
			}
			else
				Error();
			
		}
		else
			Error();
		
	}
	else
		Error();
}
