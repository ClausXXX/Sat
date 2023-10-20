#ifndef CORECONSTANTS_H
#define CORECONSTANTS_H

// ���� ����� �����
#define TO_SCREEN      0
#define TO_FILE        1
#define TO_SCREEN_FILE 2

// ������������ ����� �������� � ������
#define BYTES_OF_STRING 512

#define TYPES_OF_SYSTEMS 7
#define MAX_OF_SYSTEMS 4
#define MAX_OF_UNKNOWNS MAX_OF_SYSTEMS + 3
#define MAX_OF_VISIBLE_SATS 15 * MAX_OF_SYSTEMS
#define MAX_TYPES_OF_OBSERV 32

// ������� ����������������� �������� �������
#define INTERPOLATION_ORDER 10

// �������� �����, �/�
extern const double c;
// ������ �����, ��
extern const double Re_WGS84;
// ������ ����� � BeiDou, ��
extern const double Re_BDS;
 // ����������� �������� ��� � �
extern const double RadToDeg;
// ����������� �������� � � ���
extern const double DegToRad;

// ����������� -2sqrt(mu_WGS_84)/�^2
extern const double C;
// ������� f1 (L1) GPS, ���
extern const double fL1_GPS;
// ������� f2 (L2) GPS, ���
extern const double fL2_GPS;
// ������� ��������� �������� ������ f1 � f2 GPS
extern const double gamma_GPS;
// ��������������� ��������� ��������������� ���� ����� � ������ ��������� � ������� WGS-84, �^3/�^2
extern const double mu_WGS84;
// ������� (��������������) ������� ����������� ���������� � ������� WGS-84, �
extern const double ae_WGS84;
// ������� ������� �������� �������� ����� ������������ ����� ���������
// ������������� � ������� WGS-84, ���/�
extern const double OMEGAi_WGS84;

// ������� f1 (G1) �������, ���
extern const double fG1_GLO;
// ������� f2 (G2) �������, ���
extern const double fG2_GLO;
// �������� �������� ������ ����� ��������� �������� ������� � ��������� G1
extern const double dfG1_GLO;
// �������� �������� ������ ����� ��������� �������� ������� � ��������� G2
extern const double dfG2_GLO;
// ������� ��������� �������� ������ f1 � f2 �������
extern const double gamma_GLO;
// ��������������� ��������� ��������������� ���� ����� � ������ ��������� �
// ������� ��-90.02, �^3/�^2
extern const double mu_PZ90;
// ��������� ������������� ����������� ������ �������
extern const double J20_PZ90;
// ������� (��������������) ������� ����������� ���������� � ������� ��-90.02
extern const double ae_PZ90;
// ������� ������� �������� �������� ����� ������������ ����� ���������
// ������������� � ������� ��-90.02, ���/�
extern const double omega_PZ90;
// ������� f1 (E1) GALILEO, ���
extern const double fE1_GAL;
// ������� f2 (E5a) GALILEO, ���
extern const double fE5a_GAL;
// ������� f2 (E5b) GALILEO, ���
extern const double fE5b_GAL;
// ������� ��������� �������� ������ E1 � E5a GALILEO
extern const double gamma_E1_E5a_GAL;
// ������� ��������� �������� ������ E1 � E5b GALILEO
extern const double gamma_E1_E5b_GAL;

// ������� f1 (B1I) BeiDou, ���
extern const double fB1_BDS;
// ������� f2 (B2I) BeiDou, ���
extern const double fB2_BDS;
// ������� ��������� �������� ������ B1 � B2 BeiDou
extern const double gamma_B1_B2_BDS;
// ��� �������������� ��� ������ �����-����� 4 �������, �
extern const double GLOStep;

// ������ ������������ ���� ��� ������ GEMEC, ��
extern const double hi_GEMTEC;
// ������ ������������ ���� ��� ������ BDGIM, ��
extern const double hi_BDGIM;
// ������ ������������ ���� ��� �������� TayAbsTEC, ��
extern const double hi_T;


// �������������� ������ ��������� ���������� ������, ���
extern const double phiM;
// �������������� ������� ��������� ���������� ������, ���
extern const double lambdaM;

 // ������ �������� �� ������ ����, ����
extern const double p0;
// ����������� �� ������ ����, �C
extern const double t0;
// ������������� ��������� ������� �� ������ ����, %
extern const double r0;
// �������� ������� ����������� � ����������, �/��
extern const double Beta;
// ������ ����������, ��
extern const double ht;

#endif
