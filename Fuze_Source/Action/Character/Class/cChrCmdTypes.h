// -------------------------------------------------------------------------- //
//! @file   cChrCmdTypes.h
//! @brief  �R�}���h���͌n define��`
//! @author Nal
//! @since  13/01/05(�y)
//!
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#ifndef	_CHRCMDTYPES_H
#define	_CHRCMDTYPES_H
/* ========================================================================= */
/*																			 */
/* ========================================================================= */
//!	�{�^������
#define CHRPAD_DAT	0	//!< ������Ă���
#define CHRPAD_TRG	1	//!< ������
#define CHRPAD_POP	2	//!< ������
#define CHRPAD_PRE	3	//!< ��s����
#define CHRPAD_REP	4	//!< ���s�[�g
#define CHRPAD_SAME	5	//!< ��������
#define CHRPAD_BEAT	6	//!< �A��

//!	�U���e�[�u��
#define ATKTBLF_SAME		(1<< 0)		//!< ���������p
#define ATKTBLF_EQLEVER		(1<< 1)		//!< ���o�[��r�� "=" �ōs��
#define ATKTBLF_02			(1<< 2)		//!< 
#define ATKTBLF_03			(1<< 3)		//!< 
#define ATKTBLF_04			(1<< 4)		//!< 
#define ATKTBLF_05			(1<< 5)		//!< 
#define ATKTBLF_06			(1<< 6)		//!< 
#define ATKTBLF_07			(1<< 7)		//!< 
#define ATKTBLF_08			(1<< 8)		//!< 
#define ATKTBLF_09			(1<< 9)		//!< 

/* ========================================================================= */
/*		�R�}���h�X�N���v�g													 */
/* ========================================================================= */
#define	CHRCMD_SETSPATK			(0)		//!< �������̃A�j���ԍ��ݒ�
#define	CHRCMD_SETSPATK2		(1)		//!< �������̓���U���ݒ�
#define	CHRCMD_SETCOMBOATK		(2)		//!< �������̃R���{�ݒ�
#define	CHRCMD_SETCOMBOTHROW	(3)		//!< �������̓����R���{�ݒ�
#define	CHRCMD_CHKCOMBOLIST		(4)		//!< ���͗L���ȃA�j���ԍ��̃��X�g
#define	CHRCMD_CHKPAD			(5)		//!< �p�b�h���̓`�F�b�N�i==��r�j
#define	CHRCMD_CHKPADTST		(6)		//!< �p�b�h���̓`�F�b�N�iand��r�j
#define	CHRCMD_CHKPADROLL		(7)		//!< �p�b�h���̓`�F�b�N�i��]���́j
#define	CHRCMD_CHKMP			(8)		//!< MP�`�F�b�N
#define	CHRCMD_CHKFLAG			(9)		//!< �t���O�`�F�b�N
#define	CHRCMD_CHKANMNO_N		(10)	//!< �A�j���ԍ��`�F�b�N�i�ے�j
#define	CHRCMD_CHKEX			(11)	//!< �ŗL����
#define	CHRCMD_LIMITFTST		(12)	//!< �ڍs�����ݒ� ���v�����NG &
#define	CHRCMD_LIMITFNTST		(13)	//!< �ڍs�����ݒ� ���v�����NG !&
#define	CHRCMD_LIMITFEQ			(14)	//!< �ڍs�����ݒ� ���v�����NG ==
#define	CHRCMD_LIMITMP			(15)	//!< �ڍs�����ݒ� MP
#define	CHRCMD_LIMITANMNO		(16)	//!< �ڍs�����ݒ� �A�j���ԍ�
#define	CHRCMD_LIMITEX			(17)	//!< �ڍs�����ݒ� �ŗL�����R�[���o�b�N�ݒ�
#define	CHRCMD_PRINT			(18)	//!< ���O�o��
#define	CHRCMD_END				(19)	//!< �e�[�u���I��
#define	CHRCMD_MAX				(20)	//!< 

//!		SPATK	
//!	���
#define SPATKKIND_BASEATK		(0)	//!< �ʏ�								
#define SPATKKIND_LEVATK		(1)	//!< ���o�[								
#define SPATKKIND_SPATK			(2)	//!< �K�E�Z								
#define SPATKKIND_EXATK			(3)	//!< ���K�E�Z							
//!	����
#define SPATKATTR_NONE			(0)	//!< �w��Ȃ�							
#define SPATKATTR_GROUND		(1)	//!< �n��Z								
#define SPATKATTR_AIR			(2)	//!< �󒆋Z								
//! �󋵐���
#define SPATKONLY_NONE			(0)	//!< ���ɂȂ�							
#define SPATKONLY_HIT			(1)	//!< �q�b�g�L�����Z���ł̂ݏo����Z		
#define SPATKONLY_SP			(2)	//!< �X�[�p�[�L�����Z���̂ݏo����Z		
#define SPATKONLY_GD			(3)	//!< �K�[�h�L�����Z���ł̂ݏo����Z		
#define SPATKONLY_DMG			(4)	//!< �_���[�W�L�����Z���ł̂ݏo����Z	
#define SPATKONLY_ETCF			(5)	//!< ETCF_CANCEL�ł̂ݏo����Z			
//! �t���O
#define SPATKF_CAN_DISABLE		(1<<0)	//!< �L�����Z���s��						
#define SPATKF_CAN_SP			(1<<1)	//!< �X�[�p�[�L�����Z���ł��o����Z		
#define SPATKF_CAN_GD			(1<<2)	//!< �K�[�h�L�����Z���ł��o����Z		
#define SPATKF_CAN_DMG			(1<<3)	//!< �_���[�W�L�����Z���ł��o����Z		
#define SPATKF_CAN_GUARDIDLE	(1<<4)	//!< �K�[�h�ڍs							
#define SPATKF_CAN_ALWAYS		(1<<5)	//!< �L�����Z���\�Z�ȊO�ł��L�����Z���ŏo����	
#define SPATKF_CAN_NOHIT		(1<<6)	//!< �Z���q�b�g���Ă��Ȃ��Ă��L�����Z���ŏo����i��L�����Z���j	
#define SPATKF_CAN_FAST			(1<<7)	//!< �U�����蔭���O�ɃL�����Z���ł���		
#define SPATKF_SAME				(1<<8)	//!< ��������								
#define SPATKF_NCLEAR			(1<<9)	//!< ���͐������ɐ�s���̓N���A���Ȃ�		
#define SPATKF_NCLEAR2			(1<<10)	//!< ����SPATK���͐������ɓ��̓N���A����Ȃ�	
#define SPATKF_MOVE				(1<<11)	//!< �ړ��Z�����i�ړ��������ɏo���Ȃ��j	
#define SPATKF_SATK_N			(1<<12)	//!< ����U�� N							
#define SPATKF_SATK_U			(1<<13)	//!< ����U�� ��						
#define SPATKF_SATK_LR			(1<<14)	//!< ����U�� ����						
#define SPATKF_SATK_D			(1<<15)	//!< ����U�� ��						
#define SPATKF_COMBO_NOWAITEND	(1<<16)	//!< �R���{ ����I����҂��Ȃ�			

//!	�ŗL����
#define	SPATKCHKEX_CHRBIT_LOCKONTARGET		(0)	//!< �L�r�b�g�@ ���b�N�I���^�[�Q�b�g�L
#define	SPATKCHKEX_CHRBIT_NOLOCKONTARGET	(1)	//!< �L�r�b�g�@ ���b�N�I���^�[�Q�b�g��
#define	SPATKCHKEX_CHRBIT_FULLLOCKONTARGET	(2)	//!< �L�r�b�g�@ �^�[�Q�b�g�֑S�e���b�N�I��
#define	SPATKCHKEX_CHRBIT_LOCKONSEARCH		(3)	//!< �L�r�b�g�@ ���b�N�I���T�[�`�o���Ă�
//!	�ŗL�����R�[���o�b�NID
#define	SPATKEX_PLAYEREX		(0)	//!< Player EX�A�^�b�N
#define	SPATKEX_PLAYEREXFULL	(1)	//!< Player EX����
#define	SPATKEX_PLAYERDASH		(2)	//!< Player �_�b�V��(�A�����W)


#endif	/*	_CHRCMDTYPES_H	*/
//=============================================================================
// eof
//=============================================================================
