#include "main.h"

float PID_Calc(PID *P, float ActualValue)
{
	P->PreError = P->SetPoint - ActualValue;
	P->dError = P->PreError - P->LastError;
	
	P->SetPointLast = P->SetPoint;

	P->SumError += P->PreError;
	P->LastError = P->PreError;
	
	if(P->SumError >= P->IMax)
		P->SumError = P->IMax;
	else if(P->SumError <= -P->IMax)
		P->SumError = -P->IMax;
	
	P->POut = P->P * P->PreError;
	P->IOut = P->I * P->SumError;
	P->DOut = P->D * P->dError;
	
	return P->POut+P->IOut+P->DOut;
}

