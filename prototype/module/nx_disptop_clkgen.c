/*                                                                              
 *      Copyright (C) 2012 Nexell Co., All Rights Reserved                      
 *      Nexell Co. Proprietary & Confidential                                   
 *                                                                              
 *      NEXELL INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE  
 *      AND WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
 *      BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS
 *      FOR A PARTICULAR PURPOSE.                                               
 *                                                                              
 *      Module          : DISPTOP Clkgen
 *      File            : nx_disptop_clkgen.c
 *      Description     : 
 *      Author          : Firware Team
 *      History         : 
 */ 
#include "nx_disptop_clkgen.h"
#include "nx_displaytop.h"

static	struct
{
	struct NX_DISPTOP_CLKGEN_RegisterSet *__g_pRegister;

} __g_ModuleVariables[NUMBER_OF_DISPTOP_CLKGEN_MODULE] = { {CNULL, }, };

CBOOL	NX_DISPTOP_CLKGEN_Initialize( void )
{
	static CBOOL bInit = CFALSE;
	U32 i;

	if( CFALSE == bInit )
	{
		for( i=0; i < NUMBER_OF_DISPTOP_CLKGEN_MODULE; i++ )
		{
			__g_ModuleVariables[i].__g_pRegister = CNULL;
		}

		bInit = CTRUE;
	}

	return CTRUE;
}

U32	NX_DISPTOP_CLKGEN_GetNumberOfModule( void )
{
	return NUMBER_OF_DISPTOP_CLKGEN_MODULE;
}
U32	NX_DISPTOP_CLKGEN_GetPhysicalAddress( U32 ModuleIndex )
{
	static const U32 PhysicalAddr[] = { PHY_BASEADDR_LIST( DISPTOP_CLKGEN ) }; // PHY_BASEADDR_DISPTOP_CLKGEN_MODULE
	NX_CASSERT( NUMBER_OF_DISPTOP_CLKGEN_MODULE == (sizeof(PhysicalAddr)/sizeof(PhysicalAddr[0])) );
	NX_ASSERT( NUMBER_OF_DISPTOP_CLKGEN_MODULE > ModuleIndex );
	//NX_ASSERT( PHY_BASEADDR_CLKGEN0_MODULE == PhysicalAddr[0] );
	//NX_ASSERT( PHY_BASEADDR_CLKGEN1_MODULE == PhysicalAddr[1] );
	//NX_ASSERT( PHY_BASEADDR_CLKGEN2_MODULE == PhysicalAddr[2] );
	// ...

	return (U32)PhysicalAddr[ModuleIndex];
}
U32	NX_DISPTOP_CLKGEN_GetSizeOfRegisterSet( void )
{
	return sizeof(struct NX_DISPTOP_CLKGEN_RegisterSet);
}
void	NX_DISPTOP_CLKGEN_SetBaseAddress( U32 ModuleIndex, void* BaseAddress )
{
	NX_ASSERT( CNULL != BaseAddress );
	NX_ASSERT( NUMBER_OF_DISPTOP_CLKGEN_MODULE > ModuleIndex );
	__g_ModuleVariables[ModuleIndex].__g_pRegister = (struct NX_DISPTOP_CLKGEN_RegisterSet *)BaseAddress;
}
void*	NX_DISPTOP_CLKGEN_GetBaseAddress( U32 ModuleIndex )
{
	NX_ASSERT( NUMBER_OF_DISPTOP_CLKGEN_MODULE > ModuleIndex );
	return (void*)__g_ModuleVariables[ModuleIndex].__g_pRegister;
}


void	NX_DISPTOP_CLKGEN_SetClockBClkMode( U32 ModuleIndex, NX_BCLKMODE mode )
{
	//const U32 PCLKMODE_POS	=	0;

	register U32 regvalue;
	register struct NX_DISPTOP_CLKGEN_RegisterSet* __g_pRegister;

	U32 clkmode=0;

	NX_ASSERT( NUMBER_OF_DISPTOP_CLKGEN_MODULE > ModuleIndex );
	NX_ASSERT( CNULL != __g_ModuleVariables[ModuleIndex].__g_pRegister );

	__g_pRegister = __g_ModuleVariables[ModuleIndex].__g_pRegister;

	switch(mode)
	{
		case NX_BCLKMODE_DISABLE:	clkmode = 0;
		case NX_BCLKMODE_DYNAMIC:	clkmode = 2;		break;
		case NX_BCLKMODE_ALWAYS:	clkmode = 3;		break;
		default: NX_ASSERT( CFALSE );
	}

	regvalue = __g_pRegister->CLKENB;

	regvalue &= ~3UL;
	regvalue |= ( clkmode & 0x03 );

	//	__g_pRegister->CLKENB = regvalue;
	WriteIO32(&__g_pRegister->CLKENB, regvalue);
}

NX_BCLKMODE	NX_DISPTOP_CLKGEN_GetClockBClkMode( U32 ModuleIndex )
{
	//const U32 PCLKMODE_POS	= 0;
	U32 mode=0;

	NX_ASSERT( NUMBER_OF_DISPTOP_CLKGEN_MODULE > ModuleIndex );
	NX_ASSERT( CNULL != __g_ModuleVariables[ModuleIndex].__g_pRegister );

	mode = ( __g_ModuleVariables[ModuleIndex].__g_pRegister->CLKENB & 3UL );
	switch(mode)
	{
		case 0: return NX_BCLKMODE_DISABLE;
		case 2: return NX_BCLKMODE_DYNAMIC;
		case 3: return NX_BCLKMODE_ALWAYS ;
		default: NX_ASSERT( CFALSE );
	}
	return	NX_BCLKMODE_DISABLE;
}


void	NX_DISPTOP_CLKGEN_SetClockPClkMode( U32 ModuleIndex, NX_PCLKMODE mode )
{
	const U32 PCLKMODE_POS	=	3;

	register U32 regvalue;
	register struct NX_DISPTOP_CLKGEN_RegisterSet* __g_pRegister;

	U32 clkmode=0;

	NX_ASSERT( NUMBER_OF_DISPTOP_CLKGEN_MODULE > ModuleIndex );
	NX_ASSERT( CNULL != __g_ModuleVariables[ModuleIndex].__g_pRegister );

	__g_pRegister = __g_ModuleVariables[ModuleIndex].__g_pRegister;

	switch(mode)
	{
		case NX_PCLKMODE_DYNAMIC:	clkmode = 0;		break;
		case NX_PCLKMODE_ALWAYS:	clkmode = 1;		break;
		default: NX_ASSERT( CFALSE );
	}

	regvalue = __g_pRegister->CLKENB;

	regvalue &= ~(1UL<<PCLKMODE_POS);
	regvalue |= ( clkmode & 0x01 ) << PCLKMODE_POS;

	//	__g_pRegister->CLKENB = regvalue;
	WriteIO32(&__g_pRegister->CLKENB, regvalue);
}

//------------------------------------------------------------------------------
/**
 *	@brief		Get current PCLK mode
 *	@param[in]	ModuleIndex		An index of module ( 0 ~ 1 ).
 *	@return		Current PCLK mode
 */
NX_PCLKMODE	NX_DISPTOP_CLKGEN_GetClockPClkMode( U32 ModuleIndex )
{
	const U32 PCLKMODE_POS	= 3;

	NX_ASSERT( NUMBER_OF_DISPTOP_CLKGEN_MODULE > ModuleIndex );
	NX_ASSERT( CNULL != __g_ModuleVariables[ModuleIndex].__g_pRegister );

	if( __g_ModuleVariables[ModuleIndex].__g_pRegister->CLKENB & ( 1UL << PCLKMODE_POS ) )
	{
		return NX_PCLKMODE_ALWAYS;
	}

	return	NX_PCLKMODE_DYNAMIC;
}


//------------------------------------------------------------------------------
/**
 *	@brief		Set clock source of clock generator
 *	@param[in]	ModuleIndex	An index of module ( 0 ~ 1 ).
 *	@param[in]	Index		Select clock generator( 0 : clock generator 0 );
 *	@param[in]	ClkSrc		Select clock source of clock generator ( 0: PLL0, 1:PLL1 ).
 *	@remarks	CLKGEN have one clock generator. so \e Index must set to 0.
 *	@return		None.
 */
void	NX_DISPTOP_CLKGEN_SetClockSource( U32 ModuleIndex, U32 Index, U32 ClkSrc )
{
	const U32 CLKSRCSEL_POS		= 2;
	const U32 CLKSRCSEL_MASK	= 0x07 << CLKSRCSEL_POS;

	register U32 ReadValue;

	NX_ASSERT( NUMBER_OF_DISPTOP_CLKGEN_MODULE > ModuleIndex );
	//NX_ASSERT( 0 == Index );
	//NX_ASSERT( 2 > ClkSrc );
	NX_ASSERT( CNULL != __g_ModuleVariables[ModuleIndex].__g_pRegister );

	ReadValue = __g_ModuleVariables[ModuleIndex].__g_pRegister->CLKGEN[Index<<1];

	ReadValue &= ~CLKSRCSEL_MASK;
	ReadValue |= ClkSrc << CLKSRCSEL_POS;

	//	__g_ModuleVariables[ModuleIndex].__g_pRegister->CLKGEN[Index<<1] = ReadValue;
	WriteIO32(&__g_ModuleVariables[ModuleIndex].__g_pRegister->CLKGEN[Index<<1], ReadValue);
}

//------------------------------------------------------------------------------
/**
 *	@brief		Get clock source of specified clock generator.
 *	@param[in]	ModuleIndex	An index of module ( 0 ~ 1 ).
 *	@param[in]	Index		Select clock generator( 0 : clock generator 0 );
 *	@return		Clock source of clock generator ( 0:PLL0, 1:PLL1 ).
 *	@remarks	CLKGEN have one clock generator. so \e Index must set to 0.
 */
U32	NX_DISPTOP_CLKGEN_GetClockSource( U32 ModuleIndex, U32 Index )
{
	const U32 CLKSRCSEL_POS		= 2;
	const U32 CLKSRCSEL_MASK	= 0x07 << CLKSRCSEL_POS;

	NX_ASSERT( NUMBER_OF_DISPTOP_CLKGEN_MODULE > ModuleIndex );
	//NX_ASSERT( 0 == Index );
	NX_ASSERT( CNULL != __g_ModuleVariables[ModuleIndex].__g_pRegister );

	return ( __g_ModuleVariables[ModuleIndex].__g_pRegister->CLKGEN[Index<<1] & CLKSRCSEL_MASK ) >> CLKSRCSEL_POS;
}

//------------------------------------------------------------------------------
/**
 *	@brief		Set clock divisor of specified clock generator.
 *	@param[in]	ModuleIndex	An index of module ( 0 ~ 1 ).
 *	@param[in]	Index		Select clock generator( 0 : clock generator 0 )
 *	@param[in]	Divisor		Clock divisor ( 1 ~ 256 ).
 *	@return		None.
 *	@remarks	CLKGEN have one clock generator. so \e Index must set to 0.
 */
void	NX_DISPTOP_CLKGEN_SetClockDivisor( U32 ModuleIndex, U32 Index, U32 Divisor )
{
	const U32 CLKDIV_POS	=	5;
	const U32 CLKDIV_MASK	=	0xFF << CLKDIV_POS;

	register U32 ReadValue;

	NX_ASSERT( NUMBER_OF_DISPTOP_CLKGEN_MODULE > ModuleIndex );
	//NX_ASSERT( 0 == Index );
	NX_ASSERT( 1 <= Divisor && Divisor <= 256 );
	NX_ASSERT( CNULL != __g_ModuleVariables[ModuleIndex].__g_pRegister );

	ReadValue	=	__g_ModuleVariables[ModuleIndex].__g_pRegister->CLKGEN[Index<<1];

	ReadValue	&= ~CLKDIV_MASK;
	ReadValue	|= (Divisor-1) << CLKDIV_POS;

	WriteIO32(&__g_ModuleVariables[ModuleIndex].__g_pRegister->CLKGEN[Index<<1], ReadValue);
}

//------------------------------------------------------------------------------
/**
 *	@brief		Get clock divisor of specified clock generator.
 *	@param[in]	ModuleIndex	An index of module ( 0 ~ 1 ).
 *	@param[in]	Index		Select clock generator( 0 : clock generator 0	);
 *	@return		Clock divisor ( 1 ~ 256 ).
 *	@remarks	CLKGEN have one clock generator. so \e Index must set to 0.
 */
U32	NX_DISPTOP_CLKGEN_GetClockDivisor( U32 ModuleIndex, U32 Index )
{
	const U32 CLKDIV_POS	=	5;
	const U32 CLKDIV_MASK	=	0xFF << CLKDIV_POS;

	NX_ASSERT( NUMBER_OF_DISPTOP_CLKGEN_MODULE > ModuleIndex );
	//NX_ASSERT( 0 == Index );
	NX_ASSERT( CNULL != __g_ModuleVariables[ModuleIndex].__g_pRegister );

	return ((__g_ModuleVariables[ModuleIndex].__g_pRegister->CLKGEN[Index<<1] & CLKDIV_MASK) >> CLKDIV_POS) + 1;
}

//------------------------------------------------------------------------------
/**
 *	@brief		Set clock generator's operation
 *	@param[in]	ModuleIndex		An index of module ( 0 ~ 1 ).
 *	@param[in]	Enable	\b CTRUE	indicates that Enable of clock generator. \n
 *						\b CFALSE	indicates that Disable of clock generator.
 *	@return		None.
 */
void	NX_DISPTOP_CLKGEN_SetClockDivisorEnable( U32 ModuleIndex, CBOOL Enable )
{
	const U32	CLKGENENB_POS	=	2;
	const U32	CLKGENENB_MASK	=	1UL << CLKGENENB_POS;

	register U32 ReadValue;

	NX_ASSERT( NUMBER_OF_DISPTOP_CLKGEN_MODULE > ModuleIndex );
	NX_ASSERT( (0==Enable) ||(1==Enable) );
	NX_ASSERT( CNULL != __g_ModuleVariables[ModuleIndex].__g_pRegister );

	ReadValue	=	__g_ModuleVariables[ModuleIndex].__g_pRegister->CLKENB;
	//ReadValue	=	NX_DISPTOP_CLKGEN_GetClockDivisorEnable(ModuleIndex);
	ReadValue	&=	~CLKGENENB_MASK;
	ReadValue	|= (U32)Enable << CLKGENENB_POS;

	//	__g_ModuleVariables[ModuleIndex].__g_pRegister->CLKENB	=	ReadValue;
	WriteIO32(&__g_ModuleVariables[ModuleIndex].__g_pRegister->CLKENB, ReadValue);
}

//------------------------------------------------------------------------------
/**
 *	@brief		Get status of clock generator's operation
 *	@param[in]	ModuleIndex		An index of module ( 0 ~ 1 ).
 *	@return		CTRUE	indicates that Clock generator is enabled. \n
 *			CFALSE	indicates that Clock generator is disabled.
 */
CBOOL			NX_DISPTOP_CLKGEN_GetClockDivisorEnable( U32 ModuleIndex )
{
	const U32	CLKGENENB_POS	=	2;
	const U32	CLKGENENB_MASK	=	1UL << CLKGENENB_POS;

	NX_ASSERT( NUMBER_OF_DISPTOP_CLKGEN_MODULE > ModuleIndex );
	NX_ASSERT( CNULL != __g_ModuleVariables[ModuleIndex].__g_pRegister );

	return	(CBOOL)( (__g_ModuleVariables[ModuleIndex].__g_pRegister->CLKENB & CLKGENENB_MASK) >> CLKGENENB_POS );
}

void			NX_DISPTOP_CLKGEN_SetClockOutInv( U32 ModuleIndex, U32 Index, CBOOL OutClkInv )
{
	const U32 OUTCLKINV_POS	=	1;
	const U32 OUTCLKINV_MASK	=	1UL << OUTCLKINV_POS;

	register U32 ReadValue;

	//NX_ASSERT( 2 > Index );
	//NX_ASSERT( (0==OutClkInv) ||(1==OutClkInv) );
	NX_ASSERT( CNULL != __g_ModuleVariables[ModuleIndex].__g_pRegister );

	ReadValue	=	__g_ModuleVariables[ModuleIndex].__g_pRegister->CLKGEN[Index<<1];

	ReadValue	&=	~OUTCLKINV_MASK;
	ReadValue	|=	OutClkInv << OUTCLKINV_POS;

	//__g_ModuleVariables[ModuleIndex].__g_pRegister->CLKGEN[Index<<1]	=	ReadValue;
	WriteIO32(&__g_ModuleVariables[ModuleIndex].__g_pRegister->CLKGEN[Index<<1], ReadValue);
}

CBOOL			NX_DISPTOP_CLKGEN_GetClockOutInv( U32 ModuleIndex, U32 Index )
{
	const U32 OUTCLKINV_POS		=	1;
	const U32 OUTCLKINV_MASK	=	1UL << OUTCLKINV_POS;

	NX_ASSERT( 2 > Index );
	NX_ASSERT( CNULL != __g_ModuleVariables[ModuleIndex].__g_pRegister );

	return (CBOOL)((__g_ModuleVariables[ModuleIndex].__g_pRegister->CLKGEN[Index<<1] & OUTCLKINV_MASK ) >> OUTCLKINV_POS);
}

//void	NX_DISPTOP_CLKGEN_SetClockInInv( U32 ModuleIndex, CBOOL InClkInv )
//{
//	const U32 INCLKINV_POS	=	4;
//	const U32 INCLKINV_MASK	=	1UL << INCLKINV_POS;
//
//	register U32 ReadValue;
//
//	NX_ASSERT( (0==InClkInv) ||(1==InClkInv) );
//	NX_ASSERT( CNULL != __g_ModuleVariables[ModuleIndex].__g_pRegister )
//
//	ReadValue	=	__g_ModuleVariables[ModuleIndex].__g_pRegister->CLKENB;
//
//	ReadValue	&=	~INCLKINV_MASK;
//	ReadValue	|=	InClkInv << INCLKINV_POS;
//
//	__g_ModuleVariables[ModuleIndex].__g_pRegister->CLKENB	=	ReadValue;
//	WriteIO32(&__g_ModuleVariables[ModuleIndex].__g_pRegister->CLKENB, ReadValue);
//}
//
//CBOOL	NX_DISPTOP_CLKGEN_GetClockInInv( U32 ModuleIndex)
//{
//	const U32 INCLKINV_POS		=	4;
//	const U32 INCLKINV_MASK	=	1UL << INCLKINV_POS;
//
//	NX_ASSERT( CNULL != __g_ModuleVariables[ModuleIndex].__g_pRegister );
//
//	return (CBOOL)((__g_ModuleVariables[ModuleIndex].__g_pRegister->CLKENB & INCLKINV_MASK ) >> INCLKINV_POS);
//}

CBOOL	NX_DISPTOP_CLKGEN_SetInputInv( U32 ModuleIndex, U32 Index, CBOOL InClkInv )
{
	const U32 INCLKINV_POS	=	4 + Index;
	const U32 INCLKINV_MASK	=	1UL << INCLKINV_POS;

	register U32 ReadValue;

	NX_ASSERT( (0==InClkInv) ||(1==InClkInv) );
	NX_ASSERT( CNULL != __g_ModuleVariables[ModuleIndex].__g_pRegister )

		ReadValue	=	__g_ModuleVariables[ModuleIndex].__g_pRegister->CLKENB;

	ReadValue	&=	~INCLKINV_MASK;
	ReadValue	|=	InClkInv << INCLKINV_POS;

	//__g_ModuleVariables[ModuleIndex].__g_pRegister->CLKENB	=	ReadValue;
	WriteIO32(&__g_ModuleVariables[ModuleIndex].__g_pRegister->CLKENB, ReadValue);
	return CTRUE;
}

CBOOL	NX_DISPTOP_CLKGEN_GetInputInv( U32 ModuleIndex, U32 Index )
{
	const U32 INCLKINV_POS	=	4 + Index;
	const U32 INCLKINV_MASK	=	1UL << INCLKINV_POS;

	NX_ASSERT( CNULL != __g_ModuleVariables[ModuleIndex].__g_pRegister );

	return (CBOOL)((__g_ModuleVariables[ModuleIndex].__g_pRegister->CLKENB & INCLKINV_MASK ) >> INCLKINV_POS);

}

void NX_DISPTOP_CLKGEN_SetClockOutSelect( U32 ModuleIndex, U32 Index, CBOOL bBypass )
{
	//	const U32 OUTCLKSEL_POS		=	0;
	//	const U32 OUTCLKSEL_MASK	=	1UL << OUTCLKSEL_POS;

	//	register struct NX_DPC_RegisterSet*	pRegister;
	register U32 	ReadValue;


	NX_ASSERT( Index == 1 );
	NX_ASSERT( (0==bBypass) ||(1==bBypass) );
	NX_ASSERT( CNULL != __g_ModuleVariables[ModuleIndex].__g_pRegister )

		ReadValue	=	__g_ModuleVariables[ModuleIndex].__g_pRegister->CLKGEN[Index<<1];

	ReadValue	=	ReadValue & (~0x01);
	ReadValue	=   ReadValue | bBypass;

	//__g_ModuleVariables[ModuleIndex].__g_pRegister->CLKENB	=	ReadValue;
	WriteIO32(&__g_ModuleVariables[ModuleIndex].__g_pRegister->CLKGEN[Index<<1], ReadValue);
}
