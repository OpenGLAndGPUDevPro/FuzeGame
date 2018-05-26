// -------------------------------------------------------------------------- //
//! @file   AttackTypes.h
//! @brief  �U���Ǘ���`
//! @author Nal
//! @since  11/10/01(�y)
//!
//! COPYRIGHT (C) 2011 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#ifndef	_ATTACKTYPES_H
#define	_ATTACKTYPES_H
/* ========================================================================= */
/*		��` 																 */
/* ========================================================================= */
//!	�t���O�iHIT�v���~�e�B�u�̃��[�U�[�t���O�j
#define	COLUSERF_ATK		(1<< 0)	//!< �U������
#define	COLUSERF_DMG		(1<< 1)	//!< �_���[�W����
#define	COLUSERF_PUSH		(1<< 2)	//!< ������������
#define	COLUSERF_SCALE		(1<< 3)	//!< �}�g���N�X�A�N�Z�T�̃X�P�[���L��

/* ========================================================================= */
/*		��`�i�p�����[�^�֘A�j												 */
/* ========================================================================= */
//!	�З�
#define	ATKPOW_W	( 0)	//!< ��
#define	ATKPOW_M	( 1)	//!< ��
#define	ATKPOW_S	( 2)	//!< ��
#define	ATKPOW_MAX	( 3)

//!	�U���ӏ�
#define	ATKPNT_HIGH		( 0)	//!< ��i
#define	ATKPNT_MID		( 1)	//!< ���i
#define	ATKPNT_LOW		( 2)	//!< ���i
#define	ATKPNT_MAX		( 3)

//!	���x�i���E�D��x�j
#define	ATKHARD_HAND	( 0)	//!< �f��			�i�n���ɕ����� / ����R��Ȃǂ̓��e�U���j
#define	ATKHARD_WEAPON	( 1)	//!< ����			�i�f��Ƃ��n���Ƃ����E���� / �_�A�n���ɂ������Ȃ��S���Ȃǁj
#define	ATKHARD_EDGE	( 2)	//!< �n��			�i�f��ɏ��� / ���j
#define	ATKHARD_ENERGY	( 3)	//!< ��̃G�l���M�[	�iHAND�AWEAPON�AEDGE�ɗD�悵�ď��� / ���@�A���p�j
#define	ATKHARD_MAX		( 4)

//!	�V���b�g���E���x��
#define	SHOTERASE_NONE	( 0)	//!< 
#define	SHOTERASE_L1	( 1)	//!< ���^�e�܂ŏ�����
#define	SHOTERASE_L2	( 2)	//!< �r�[���܂ŏ�����
#define	SHOTERASE_MAX	( 3)

//!	�q�b�g�̎d���i�G�t�F�N�g�Ɋւ�� / mHitKind�j
#define	HITKIND_HIT		( 0)	//!< �q�b�g
#define	HITKIND_GUARD	( 1)	//!< �K�[�h
#define	HITKIND_FIELD	( 2)	//!< ���E��
#define	HITKIND_SOUSAI	( 3)	//!< ���E
#define	HITKIND_MAX		( 4)

//!	�_���[�W�X�e�[�^�X�^�C�v�i�_���[�W���[�V�����e�� / ���قǗD��x�������j
#define	ATKDMGSTA_NONE		( 0)	//!< �w�薳��
#define	ATKDMGSTA_NODMG		( 1)	//!< �_���[�W���[�V��������
#define	ATKDMGSTA_SMASH		( 2)	//!< �@����
#define	ATKDMGSTA_BOUND		( 3)	//!< �o�E���h
#define	ATKDMGSTA_FLOAT		( 4)	//!< ����
#define	ATKDMGSTA_STRIKE	( 5)	//!< �������
#define	ATKDMGSTA_VROLL		( 6)	//!< �c��]�������
#define	ATKDMGSTA_MAX		( 7)

//!	�t���O �U���Ǘ��i���[�V�����e����L�����̃A�N�V���������Ɋ֘A����j
#define	ATKF_CONTINUE			(1<< 0)	//!< ���i�U���̃A�j���ŁA�܂��I���U���ł͂Ȃ��i�R���{���͏����ŕK�v�B�I���o�߂��i�܂Ȃ��j
#define	ATKF_CANCEL				(1<< 1)	//!< ����L�����Z���� 
#define	ATKF_JCANCEL			(1<< 2)	//!< �W�����v�L�����Z���\�iATKF_CANCEL�Ƃ͓Ɨ����ċ@�\�j
#define	ATKF_AIR				(1<< 3)	//!< �󒆍U���i�n��̓G�ɓ�����Ȃ��j
#define	ATKF_HIGH				(1<< 4)	//!< ��i�U���i���Ⴊ�݂̓G�ɓ�����Ȃ��j
#define	ATKF_GROUND				(1<< 5)	//!< �n��U���i�󒆂̓G�ɓ�����Ȃ��j
#define	ATKF_NOHITSTEP			(1<< 6)	//!< �X�e�b�v���̑���ɓ�����Ȃ�
#define	ATKF_NODOWNESC			(1<< 7)	//!< �_�E�����s��
#define	ATKF_NOCHASE			(1<< 8)	//!< ���̍U�����󒆂��ꒆ�̑���ɓ��������ꍇ�A�ȍ~�ǌ��͕s��
#define	ATKF_NOSPARMOR			(1<< 9)	//!< �X�[�p�[�A�[�}�[�����U��
#define	ATKF_DIRREV				(1<<10)	//!< �q�b�g�����ɂ��_���[�W�����̔��]��L���iATRF_SHOT���̂ݗL���j
#define	ATKF_OTHERDIR			(1<<11)	//!< �U�������Z�o���@�̐ؑ�
#define	ATKF_BACKDIR			(1<<12)	//!< �U�������Z�o���Ɍ�납��ɌŒ�
#define	ATKF_ANMDIR				(1<<13)	//!< �A�j�����������̌���������v�Z�Ɏg�p
#define	ATKF_ROTDIR				(1<<14)	//!< �U�������Z�o���ɕ���������v�Z
#define	ATKF_NOTAERIALSPD		(1<<15)	//!< �󒆃R���{���̐����ړ��␳�𖳌�
#define	ATKF_FLOATDMGHIT		(1<<16)	//!< �󒆃_���[�W���̓G�ɂ̂݃q�b�g
#define	ATKF_FINISHHITSTOP		(1<<17)	//!< �g�h�����������������q�b�g�X�g�b�v
#define	ATKF_ABSHITSTOP			(1<<18)	//!< �����q�b�g�X�g�b�v
#define	ATKF_SHOTMTX			(1<<19)	//!< �V���b�g�}�g���N�X�g�p
#define	ATKF_HIT3D				(1<<20)	//!< ����v�Z����3D�v�Z����
#define	ATKF_21					(1<<21)	//!< 
#define	ATKF_22					(1<<22)	//!< 
#define	ATKF_23					(1<<23)	//!< 
#define	ATKF_24					(1<<24)	//!< 
#define	ATKF_25					(1<<25)	//!< 
#define	ATKF_26					(1<<26)	//!< 
#define	ATKF_27					(1<<27)	//!< 
#define	ATKF_28					(1<<28)	//!< 
#define	ATKF_29					(1<<29)	//!< 
#define	ATKF_30					(1<<30)	//!< 
#define	ATKF_GENERAL0			(1<<31)	//!< �������i�q�b�g�E�K�[�h�Ɋւ�炸�j��DMGF_GENERAL0�𗧂Ă�

#define	ATKFS_TGSMASK			(0)	//!< �A�j���X�N���v�g����̍U���ݒ莞�ɏ�����Ȃ��t���O

//!	�t���O �U�������Ǘ��i�����Ɋւ��j
#define	ATRF_EX					(1<< 0)	//!< ���K�E
#define	ATRF_SHOT				(1<< 1)	//!< ��ѓ���
#define	ATRF_THROW				(1<< 2)	//!< ����
#define	ATRF_GRAPPLE			(1<< 3)	//!< �Ō������iATRF_THROW�Ɠ����ɗ��K�v������j
#define	ATRF_ATEMI				(1<< 4)	//!< ���g
#define	ATRF_NGUARD				(1<< 5)	//!< �K�[�h�s��
#define	ATRF_ABSSOUSAI			(1<< 6)	//!< [���D��]	�U�����蓯�m�̗D�攻�莞�������E
#define	ATRF_NSOUSAI			(1<< 7)	//!<   ��		���̃t���O�ɗD�悵�đ��E����𖳌�������
#define	ATRF_SOUSAI_EX			(1<< 8)	//!< [��D��]	���x�Ɋ֌W�Ȃ��D�悵�ď��B���̃t���O���m���Ԃ����ꍇ�͖�������邽�ߋ��x�ݒ�������Ƃ��Ă�������
#define	ATRF_DOWNATK			(1<< 9)	//!< �_�E���ǂ�����
#define	ATRF_KEZURIDMG			(1<<10)	//!< ���_���[�W��^����
#define	ATRF_NHITCOUNT			(1<<11)	//!< �q�b�g�J�E���g�𑝉����Ȃ�
#define	ATRF_COMRADE			(1<<12)	//!< �����`�[���ւ̍U��
#define	ATRF_PLAYERTEAM			(1<<13)	//!< �v���C���[�`�[���ւ̍U��
#define	ATRF_ALLTEAM			(1<<14)	//!< �S�`�[���ւ̍U��
#define	ATRF_HITSHOTONLY		(1<<15)	//!< �V���b�g�ɂ̂ݓ�����
#define	ATRF_FAR				(1<<16)	//!< �����̓G�ɗL��
#define	ATRF_REQCRASH			(1<<17)	//!< �������ݔ���
#define	ATRF_REQCRASHREBOUND	(1<<18)	//!< ���˕Ԃ��Ԃ̑���ɓ�����Ɗ������ݔ���
#define	ATRF_CRASH				(1<<19)	//!< �Փ�
#define	ATRF_REBOUND			(1<<20)	//!< �Փˏ�Ԃ̑���ɓ�����ƏՓ˔��ˈ���
#define	ATRF_PLAYERATK			(1<<21)	//!< �v���C���[�̍U���͂��Q��
#define	ATRF_22					(1<<22)	//!< 
#define	ATRF_23					(1<<23)	//!< 
#define	ATRF_24					(1<<24)	//!< 
#define	ATRF_25					(1<<25)	//!< 
#define	ATRF_26					(1<<26)	//!< 
#define	ATRF_27					(1<<27)	//!< 
#define	ATRF_28					(1<<28)	//!< 
#define	ATRF_29					(1<<29)	//!< 
#define	ATRF_30					(1<<30)	//!< 
#define	ATRF_31					(1<<31)	//!< 

#define	ATRFS_TGSMASK			(0)	//!< �A�j���X�N���v�g����̍U���ݒ莞�ɏ�����Ȃ��t���O

//!	�t���O �����֌W		
#define	THRF_THROWGROUND		(1<< 0)	//!< �n�㓊���i�����ڍs���Ɏ����Ƒ���� PRGF_FLOAT ���N���A�����j
#define	THRF_THROWESC			(1<< 1)	//!< ���������\�i�����U���ɗ��Ă邱�ƂŗL���j
#define	THRF_TFAILEDBLOW		(1<< 2)	//!< �������莞�Ɏ��s�����ꍇ�A�Ō��Ƃ��ċ@�\����
#define	THRF_THROWCHKLAND		(1<< 3)	//!< �������莞�A�������n��ɂ��Ȃ��Ǝ��s
#define	THRF_THROWCHKAIR		(1<< 4)	//!< �������莞�A�������󒆂ɂ��Ȃ��Ǝ��s
#define	THRF_TDOWNCHKLAND		(1<< 5)	//!< �������莞�A���肪�n��ɂ��Ȃ��Ǝ��s
#define	THRF_TDOWNCHKAIR		(1<< 6)	//!< �������莞�A���肪�󒆂ɂ��Ȃ��Ǝ��s
#define	THRF_TDOWNSAMEANM		(1<< 7)	//!< �������莞�A���肪���Ɉڍs�A�j���ɂȂ��Ă����ꍇ���s
#define	THRF_TDOWNCHKDAMAGE		(1<< 8)	//!< �������莞�A���肪�_���[�W���łȂ��Ǝ��s
#define	THRF_TDOWNGUARDSTOP		(1<< 9)	//!< �������莞�A���肪�K�[�h�d�������Ǝ��s
#define	THRF_THROWNCHKSELF		(1<<10)	//!< �������莞�A�����̏�Ԃ𖳎�����
#define	THRF_GRAPPLECHKSPARMOR	(1<<11)	//!< �Ō��������莞�A�X�[�p�[�A�[�}�[����ɂ͎��s
#define	THRF_ATEMISHOT			(1<<12)	//!< ���g���莞�A��ѓ��������	�i�h�䑤�ɐݒ�j
#define	THRF_ATEMIEX			(1<<13)	//!< ���g���莞�A���K�E������	�i�h�䑤�ɐݒ�j
#define	THRF_ATEMINLOW			(1<<14)	//!< ���g���莞�A���i�͎��Ȃ�	�i�h�䑤�ɐݒ�j
#define	THRF_NATEMI				(1<<15)	//!< ���g���莞�A���g����Ȃ�		�i�U�����ɐݒ� / �ŗD��j
#define	THRF_CHKTDMGALL			(1<<16)	//!< ���ꑤ�̓������G���A�ʏ퓊���E�Ō����������`�F�b�N����i�U�����ɐݒ�j
#define	THRF_THROWGUARD			(1<<17)	//!< �K�[�h���ꂽ�ꍇ�ɓ������s�i�������̂ݗL���j
#define	THRF_TGUARDCHKDMG		(1<<18)	//!< THRF_THROWGUARD�L�����Ɏ������_���[�W�̏ꍇ�͓������������Ȃ��B
#define	THRF_THROWSELFABS		(1<<19)	//!< �������Ă��K�[�h����Ă������̓A�j���ڍs���s
#define	THRF_THROWTARABS		(1<<20)	//!< �������Ă��K�[�h����Ă�����̓A�j���ڍs���s
#define	THRF_THROWCHILD			(1<<21)	//!< CHIF_NTHROW�������Ă���q���ɑ΂��ē����𐬌��ł���
#define	THRF_TGRAPHITNOEFT		(1<<22)	//!< �Ō������������Ƀq�b�g�G�t�F�N�g���o���Ȃ�
#define	THRF_POWER				(1<<23)	//!< �������������ʏ�̑Ō��_���[�W���ʂ�
#define	THRF_24					(1<<24)	//!< 
#define	THRF_25					(1<<25)	//!< 
#define	THRF_26					(1<<26)	//!< 
#define	THRF_27					(1<<27)	//!< 
#define	THRF_28					(1<<28)	//!< 
#define	THRF_29					(1<<29)	//!< 
#define	THRF_30					(1<<30)	//!< 
#define	THRF_31					(1<<31)	//!< 

#define	THRFS_TGSMASK			(0)	//!< �A�j���X�N���v�g����̍U���ݒ莞�ɏ�����Ȃ��t���O

//!	���[�U�[ID
#define	ATKUSERID_NONE			(0)	//!< ����
#define	ATKUSERID_BLADE			(1)	//!< ���@ �u���[�h
#define	ATKUSERID_ESTDASHBLADE	(2)	//!< �G�X�g �_�b�V���u���[�h
#define	ATKUSERID_PORTALWEAK	(3)	//!< �|�[�^�� �R�A�O�k�̓�������̍U��

#endif	/*	_ATTACKTYPES_H	*/
//=============================================================================
// eof
//=============================================================================
