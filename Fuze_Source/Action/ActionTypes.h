// -------------------------------------------------------------------------- //
//! @file   ActionTypes.h
//! @brief  define��`
//! @author Nal
//! @since  11/11/10(��)
//!
//! COPYRIGHT (C) 2011 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#ifndef	_ACTIONTYPES_H
#define	_ACTIONTYPES_H
/* ========================================================================= */
/*		��` 																 */
/* ========================================================================= */
//!	�V�[���X�e�b�v
#define	SCENESTEP_READY		(0)	//!< �J��
#define	SCENESTEP_STAGE		(1)	//!< ����
#define	SCENESTEP_MIDBOSS	(2)	//!< ���{�X
#define	SCENESTEP_BOSS		(3)	//!< �{�X
#define	SCENESTEP_RESULT	(4)	//!< ���U���g
#define	SCENESTEP_MAX		(5)

//!	�V�[���t���O
#define	SCENEF_NOINPUT			(1<< 0)	//!< ���͕s��
#define	SCENEF_NOLIMITSCREEN	(1<< 1)	//!< ��ʓ��ړ������̖���
#define	SCENEF_NORENDER			(1<< 2)	//!< �قƂ�ǂ̕\�������i�X�N���[���o�b�t�@�`�撆�Ȃǁj
#define	SCENEF_NOGAUGE			(1<< 3)	//!< �Q�[�W��\��
#define	SCENEF_NOPLAYER			(1<< 4)	//!< �v���C���[�s�݃��[�h
#define	SCENEF_POSFIX_X			(1<< 5)	//!< X���W����
#define	SCENEF_POSFIX_Y			(1<< 6)	//!< Y���W����
#define	SCENEF_POSFIX_Z			(1<< 7)	//!< Z���W����
#define	SCENEF_TARGETLOCK		(1<< 8)	//!< �^�[�Q�b�g���b�N
#define	SCENEF_TL360			(1<< 9)	//!< �S���ʃ^�[�Q�b�g���b�N�i�^�[�Q�b�g���b�N���p���Ďg���j
#define	SCENEF_BEHIND			(1<<10)	//!< �r�n�C���h���[�h
#define	SCENEF_ABSTALK			(1<<11)	//!< �����I�ɉ�b��L���ɂ���
#define	SCENEF_PAUSEBGM			(1<<12)	//!< �|�[�Y��BGM���ꎞ��~����
#define	SCENEF_STAGECONNECT		(1<<13)	//!< �X�e�[�W���ׂ���
#define	SCENEF_LOSE				(1<<14)	//!< �s�k
#define	SCENEF_NOHIST_TENSION	(1<<15)	//!< �e���V�����O���t���L�^���Ȃ�
#define	SCENEFS_POSFIX			(SCENEF_POSFIX_X | SCENEF_POSFIX_Y | SCENEF_POSFIX_Z)	//!< ���W����

//!	�f���t���O
#define	DEMOF_ACTIVE			(1<< 0)	//!< �f����
#define	DEMOF_TIMESTOP			(1<< 1)	//!< �X�e�[�W�o�߂��~
#define	DEMOF_INPUTENABLE		(1<< 2)	//!< �����
#define	DEMOF_LIMITMOVESCREEN	(1<< 3)	//!< ��ʓ��ړ�����
#define	DEMOF_GAUGEDISP			(1<< 4)	//!< �Q�[�W�\��
#define	DEMOF_RESULT			(1<< 5)	//!< ���U���g�\����
#define	DEMOF_06				(1<< 6)	//!< 
#define	DEMOF_07				(1<< 7)	//!< 
#define	DEMOF_08				(1<< 8)	//!< 
#define	DEMOF_09				(1<< 9)	//!< 

//!	�f�������t���O
#define	EVENTF_00			(1<< 0)	//!< 
#define	EVENTF_01			(1<< 1)	//!< 
#define	EVENTF_02			(1<< 2)	//!< 
#define	EVENTF_03			(1<< 3)	//!< 
#define	EVENTF_04			(1<< 4)	//!< 
#define	EVENTF_05			(1<< 5)	//!< 
#define	EVENTF_06			(1<< 6)	//!< 
#define	EVENTF_07			(1<< 7)	//!< 
#define	EVENTF_08			(1<< 8)	//!< 
#define	EVENTF_09			(1<< 9)	//!< 
#define	EVENTF_10			(1<<10)	//!< 
#define	EVENTF_11			(1<<11)	//!< 
#define	EVENTF_12			(1<<12)	//!< 
#define	EVENTF_13			(1<<13)	//!< 
#define	EVENTF_14			(1<<14)	//!< 
#define	EVENTF_15			(1<<15)	//!< 
#define	EVENTF_16			(1<<16)	//!< 
#define	EVENTF_17			(1<<17)	//!< 
#define	EVENTF_18			(1<<18)	//!< 
#define	EVENTF_19			(1<<19)	//!< 
#define	EVENTF_20			(1<<20)	//!< 
#define	EVENTF_21			(1<<21)	//!< 
#define	EVENTF_22			(1<<22)	//!< 
#define	EVENTF_23			(1<<23)	//!< 
#define	EVENTF_24			(1<<24)	//!< 
#define	EVENTF_25			(1<<25)	//!< 
#define	EVENTF_26			(1<<26)	//!< 
#define	EVENTF_27			(1<<27)	//!< 
#define	EVENTF_28			(1<<28)	//!< 
#define	EVENTF_29			(1<<29)	//!< 
#define	EVENTF_30			(1<<30)	//!< 
#define	EVENTF_31			(1<<31)	//!< 

//!	�f���X���E
#define	DEMOSLOWF_CAMERA	(1<< 0)	//!< �J����
#define	DEMOSLOWF_EFFECT	(1<< 1)	//!< �G�t�F�N�g
#define	DEMOSLOWFS_ALL		(DEMOSLOWF_CAMERA | DEMOSLOWF_EFFECT)

//!	�X�N���[���^�C�v
#define	SCROLL_VERTICAL		(0)	//!< �c
#define	SCROLL_SIDE			(1)	//!< ��
#define	SCROLL_FRONT		(2)	//!< ����
#define	SCROLL_MAX			(3)

//!	�Q�[������̈ڍs [ACtrl.mSubStep]
#define	ACTRL_GAME_STEP_NONE			(0)	//!< ����
#define	ACTRL_GAME_STEP_GAMEOVER		(1)	//!< �Q�[���I�[�o�[
#define	ACTRL_GAME_STEP_RESULT			(2)	//!< ���U���g
#define	ACTRL_GAME_STEP_RETRY			(3)	//!< ���g���C
#define	ACTRL_GAME_STEP_TITLE			(4)	//!< �^�C�g��
//!	�Q�[���I�[�o�[����̈ڍs [ACtrl.mSubStep]
#define	ACTRL_GAMEOVER_STEP_NONE		(0)	//!< ����
#define	ACTRL_GAMEOVER_STEP_RELEASE		(1)	//!< ���
#define	ACTRL_GAMEOVER_STEP_CONTINUE	(2)	//!< �R���e�B�j���[
#define	ACTRL_GAMEOVER_STEP_RETRY		(3)	//!< ���g���C
#define	ACTRL_GAMEOVER_STEP_TITLE		(4)	//!< �^�C�g��


#endif	/*	_ACTIONTYPES_H	*/
//=============================================================================
// eof
//=============================================================================
