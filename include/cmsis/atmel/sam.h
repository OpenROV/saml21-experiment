#pragma once

// Helper macro
#define part_defined(part) (defined( __##part##__ ))

// ----------------------
// SAMD family
// ----------------------

// Revision A
#define SAMD21E15A 	( part_defined( SAMD21E15A ) )
#define SAMD21E16A 	( part_defined( SAMD21E16A ) )
#define SAMD21E17A 	( part_defined( SAMD21E17A ) )
#define SAMD21E18A 	( part_defined( SAMD21E18A ) )
#define SAMD21G15A 	( part_defined( SAMD21G15A ) )
#define SAMD21G16A 	( part_defined( SAMD21G16A ) )
#define SAMD21G17A 	( part_defined( SAMD21G17A ) )
#define SAMD21G17AU	( part_defined( SAMD21G17AU ) )
#define SAMD21G18A 	( part_defined( SAMD21G18A ) )
#define SAMD21G18AU	( part_defined( SAMD21G18AU ) )
#define SAMD21J15A 	( part_defined( SAMD21J15A ) )
#define SAMD21J16A 	( part_defined( SAMD21J16A ) )
#define SAMD21J17A 	( part_defined( SAMD21J17A ) )
#define SAMD21J18A 	( part_defined( SAMD21J18A ) )

#define SAMD21_REV_A (	SAMD21E15A || SAMD21E16A || SAMD21E17A || SAMD21E18A ||\
						SAMD21G15A || SAMD21G16A || SAMD21G17A || SAMD21G17AU ||\
						SAMD21G18A || SAMD21G18AU || SAMD21J15A || SAMD21J16A ||\
						SAMD21J17A || SAMD21J18A )

// Revision B
#define SAMD21E15B 	( part_defined( SAMD21E15B ) )
#define SAMD21E15BU ( part_defined( SAMD21E15BU ) )
#define SAMD21E15L 	( part_defined( SAMD21E15L ) )
#define SAMD21E16B 	( part_defined( SAMD21E16B ) )
#define SAMD21E16BU ( part_defined( SAMD21E16BU ) )
#define SAMD21E16L 	( part_defined( SAMD21E16L ) )
#define SAMD21G15B 	( part_defined( SAMD21G15B ) )
#define SAMD21G15L 	( part_defined( SAMD21G15L ) )
#define SAMD21G16B 	( part_defined( SAMD21G16B ) )
#define SAMD21G16L 	( part_defined( SAMD21G16L ) )
#define SAMD21J15B 	( part_defined( SAMD21J15B ) )
#define SAMD21J16B 	( part_defined( SAMD21J16B ) )

#define SAMD21_REV_B (	SAMD21E15B || SAMD21E15BU || SAMD21E15L || SAMD21E16B ||\
						SAMD21E16BU || SAMD21E16L || SAMD21G15B || SAMD21G15L ||\
						SAMD21G16B || SAMD21G16L || SAMD21J15B || SAMD21J16B )

#define SAMD_SERIES (SAMD21_REV_A || SAMD21_REV_B)

// ----------------------
// SAML family
// ----------------------

// Revision A
#define SAML21G18A 	( part_defined( SAML21G18A ) )
#define SAML21G16A 	( part_defined( SAML21G16A ) )
#define SAML21G17A 	( part_defined( SAML21G17A ) )
#define SAML21J18A 	( part_defined( SAML21J18A ) )
#define SAML21J17A 	( part_defined( SAML21J17A ) )
#define SAML21J16A 	( part_defined( SAML21J16A ) )
#define SAML21E15A 	( part_defined( SAML21E15A ) )
#define SAML21E16A 	( part_defined( SAML21E16A ) )
#define SAML21E17A 	( part_defined( SAML21E17A ) )

#define SAML21_REV_A (	SAML21G18A || SAML21G16A || SAML21G17A || SAML21J18A ||\
					 	SAML21J17A || SAML21J16A || SAML21E15A || SAML21E16A ||\
						SAML21E17A )

// Revision B
#define SAML21E15B	( part_defined( SAML21E15B ) )
#define SAML21E16B	( part_defined( SAML21E16B ) )
#define SAML21E17B	( part_defined( SAML21E17B ) )
#define SAML21E18B	( part_defined( SAML21E18B ) )
#define SAML21G16B	( part_defined( SAML21G16B ) )
#define SAML21G17B	( part_defined( SAML21G17B ) )
#define SAML21G18B	( part_defined( SAML21G18B ) )
#define SAML21J16B	( part_defined( SAML21J16B ) )
#define SAML21J17B	( part_defined( SAML21J17B ) )
#define SAML21J18B	( part_defined( SAML21J18B ) )
#define SAML21J18BU	( part_defined( SAML21J18BU ) )

#define SAML21_REV_B (	SAML21E15B || SAML21E16B || SAML21E17B || SAML21E18B ||\
						SAML21G16B || SAML21G17B || SAML21G18B || SAML21J16B ||\
						SAML21J17B || SAML21J18B || SAML21J18BU )

#define SAML_SERIES (SAML21_REV_A || SAML21_REV_B)

// ----------------------
// SAM Product Line
// ----------------------
#define SAM (SAMD_SERIES | SAML_SERIES)

// ----------------------
// Header inclusion
// ----------------------

#if SAMD_SERIES
	#include "samd.h"
#endif

#if SAML_SERIES
	#include "saml.h"
#endif