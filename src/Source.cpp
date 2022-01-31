
//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>
#include <direct.h>  // _mkdir
#include "clCRID.h"
#ifndef _countof
#define _countof(_array) (sizeof(_array)/sizeof(_array[0]))
#endif

//--------------------------------------------------
// �������16�i���Ƃ݂Ȃ��Đ��l�ɕϊ�
//--------------------------------------------------
int atoi16(const char *s){
	int r=0;
	bool sign=false;if(*s=='+'){s++;}else if(*s=='-'){sign=true;s++;}
	while(*s){
		if(*s>='0'&&*s<='9')r=(r<<4)|(*s-'0');
		else if(*s>='A'&&*s<='F')r=(r<<4)|(*s-'A'+10);
		else if(*s>='a'&&*s<='f')r=(r<<4)|(*s-'a'+10);
		else break;
		s++;
	}
	return sign?-r:r;
}

//--------------------------------------------------
// �f�B���N�g�����擾
//--------------------------------------------------
char *GetDirectory(char *directory,int size,const char *path){
	if(size>0)directory[0]='\0';
	for(int i=strlen(path)-1;i>=0;i--){
		if(path[i]=='\\'){
			if(i>size-1)i=size-1;
			memcpy(directory,path,i);
			directory[i]='\0';
			break;
		}
	}
	return directory;
}

//--------------------------------------------------
// �f�B���N�g���쐬
//--------------------------------------------------
bool DirectoryCreate(const char *directory){

	// �`�F�b�N
	if(!(directory&&*directory))return false;

	// ���΃p�X(�f�B���N�g�����̂�)
	if(!(strchr(directory,'\\')||strchr(directory,'/'))){
		return _mkdir(directory)==0;
	}

	// �f�B���N�g�����`�F�b�N
	if(directory[1]!=':'||directory[2]!='\\')return false;  // �h���C�u�L�q�̃`�F�b�N
	if(!directory[3])return false;                          // �h���C�u�ȊO�̋L�q�`�F�b�N
	if(strpbrk(directory+3,"/,:;*<|>\""))return false;      // �f�B���N�g���֎~�����̃`�F�b�N
	if(strstr(directory,"\\\\"))return false;               // �A������'\'�L���̃`�F�b�N
	if(strstr(directory," \\"))return false;                // �X�y�[�X�̌��'\'�L���̃`�F�b�N

	// �f�B���N�g���쐬
	if(_mkdir(directory)){
		char current[0x400];
		if(!GetDirectory(current,_countof(current),directory))return false;
		if(!DirectoryCreate(current))return false;
		if(_mkdir(directory))return false;
	}

	return true;
}

//--------------------------------------------------
// ���C��
//--------------------------------------------------
int main(int argc,char *argv[]){

	// �R�}���h���C�����
	unsigned int count=0;
	char *filenameOut=NULL;
	unsigned int ciphKey1=0x207DFFFF;
	unsigned int ciphKey2=0x00B8F21B;
	for(int i=1;i<argc;i++){
		if(argv[i][0]=='-'||argv[i][0]=='/'){
			switch(argv[i][1]){
			case 'o':if(i+1<argc){filenameOut=argv[++i];}break;
			case 'a':if(i+1<argc){ciphKey1=atoi16(argv[++i]);}break;
			case 'b':if(i+1<argc){ciphKey2=atoi16(argv[++i]);}break;
			}
		}else if(*argv[i]){
			argv[count++]=argv[i];
		}
	}

	// ���̓`�F�b�N
	if(!count){
		printf("Error: ���̓t�@�C�����w�肵�Ă��������B\n");
		return -1;
	}

	// ����
	for(unsigned int i=0;i<count;i++){

		// 2�ڈȍ~�̃t�@�C���́A�o�̓t�@�C�����I�v�V����������
		if(i)filenameOut=NULL;

		// �f�t�H���g�o�̓t�@�C����
		char path[0x400];
		if(!(filenameOut&&filenameOut[0])){
			strcpy_s(path,sizeof(path),argv[i]);
			char *d1=strrchr(path,'\\');
			char *d2=strrchr(path,'/');
			char *e=strrchr(path,'.');
			if(e&&d1<e&&d2<e)*e='\0';
			strcat_s(path,sizeof(path),".demux");
			filenameOut=path;
		}

		printf("%s �𕪗���...\n",argv[i]);
		DirectoryCreate(filenameOut);
		clCRID crid(ciphKey1,ciphKey2);
		if(!crid.Demux(argv[i],filenameOut,true)){
			printf("Error: �����Ɏ��s���܂����B\n");
		}

	}

	return 0;
}
