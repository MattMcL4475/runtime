// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

#include "jitpch.h"
#include "hwintrinsic.h"

#ifdef FEATURE_HW_INTRINSICS

//------------------------------------------------------------------------
// X64VersionOfIsa: Gets the corresponding 64-bit only InstructionSet for a given InstructionSet
//
// Arguments:
//    isa -- The InstructionSet ID
//
// Return Value:
//    The 64-bit only InstructionSet associated with isa
static CORINFO_InstructionSet X64VersionOfIsa(CORINFO_InstructionSet isa)
{
    switch (isa)
    {
        case InstructionSet_X86Base:
            return InstructionSet_X86Base_X64;
        case InstructionSet_SSE:
            return InstructionSet_SSE_X64;
        case InstructionSet_SSE2:
            return InstructionSet_SSE2_X64;
        case InstructionSet_SSE3:
            return InstructionSet_SSE3_X64;
        case InstructionSet_SSSE3:
            return InstructionSet_SSSE3_X64;
        case InstructionSet_SSE41:
            return InstructionSet_SSE41_X64;
        case InstructionSet_SSE42:
            return InstructionSet_SSE42_X64;
        case InstructionSet_AVX:
            return InstructionSet_AVX_X64;
        case InstructionSet_AVX2:
            return InstructionSet_AVX2_X64;
        case InstructionSet_AVX512BW:
            return InstructionSet_AVX512BW_X64;
        case InstructionSet_AVX512BW_VL:
            return InstructionSet_AVX512BW_VL_X64;
        case InstructionSet_AVX512CD:
            return InstructionSet_AVX512CD_X64;
        case InstructionSet_AVX512CD_VL:
            return InstructionSet_AVX512CD_VL_X64;
        case InstructionSet_AVX512DQ:
            return InstructionSet_AVX512DQ_X64;
        case InstructionSet_AVX512DQ_VL:
            return InstructionSet_AVX512DQ_VL_X64;
        case InstructionSet_AVX512F:
            return InstructionSet_AVX512F_X64;
        case InstructionSet_AVX512F_VL:
            return InstructionSet_AVX512F_VL_X64;
        case InstructionSet_AVXVNNI:
            return InstructionSet_AVXVNNI_X64;
        case InstructionSet_AES:
            return InstructionSet_AES_X64;
        case InstructionSet_BMI1:
            return InstructionSet_BMI1_X64;
        case InstructionSet_BMI2:
            return InstructionSet_BMI2_X64;
        case InstructionSet_FMA:
            return InstructionSet_FMA_X64;
        case InstructionSet_LZCNT:
            return InstructionSet_LZCNT_X64;
        case InstructionSet_PCLMULQDQ:
            return InstructionSet_PCLMULQDQ_X64;
        case InstructionSet_POPCNT:
            return InstructionSet_POPCNT_X64;
        case InstructionSet_X86Serialize:
            return InstructionSet_X86Serialize_X64;
        default:
            return InstructionSet_NONE;
    }
}

//------------------------------------------------------------------------
// VLVersionOfIsa: Gets the corresponding AVX512VL only InstructionSet for a given InstructionSet
//
// Arguments:
//    isa -- The InstructionSet ID
//
// Return Value:
//    The AVX512VL only InstructionSet associated with isa
static CORINFO_InstructionSet VLVersionOfIsa(CORINFO_InstructionSet isa)
{
    switch (isa)
    {
        case InstructionSet_AVX512BW:
            return InstructionSet_AVX512BW_VL;
        case InstructionSet_AVX512CD:
            return InstructionSet_AVX512CD_VL;
        case InstructionSet_AVX512DQ:
            return InstructionSet_AVX512DQ_VL;
        case InstructionSet_AVX512F:
            return InstructionSet_AVX512F_VL;
        default:
            return InstructionSet_NONE;
    }
}

//------------------------------------------------------------------------
// lookupInstructionSet: Gets the InstructionSet for a given class name
//
// Arguments:
//    className -- The name of the class associated with the InstructionSet to lookup
//
// Return Value:
//    The InstructionSet associated with className
static CORINFO_InstructionSet lookupInstructionSet(const char* className)
{
    assert(className != nullptr);
    if (className[0] == 'A')
    {
        if (strcmp(className, "Aes") == 0)
        {
            return InstructionSet_AES;
        }
        if (strcmp(className, "Avx") == 0)
        {
            return InstructionSet_AVX;
        }
        if (strcmp(className, "Avx2") == 0)
        {
            return InstructionSet_AVX2;
        }
        if (strcmp(className, "Avx512BW") == 0)
        {
            return InstructionSet_AVX512BW;
        }
        if (strcmp(className, "Avx512CD") == 0)
        {
            return InstructionSet_AVX512CD;
        }
        if (strcmp(className, "Avx512DQ") == 0)
        {
            return InstructionSet_AVX512DQ;
        }
        if (strcmp(className, "Avx512F") == 0)
        {
            return InstructionSet_AVX512F;
        }
        if (strcmp(className, "AvxVnni") == 0)
        {
            return InstructionSet_AVXVNNI;
        }
    }
    else if (className[0] == 'S')
    {
        if (strcmp(className, "Sse") == 0)
        {
            return InstructionSet_SSE;
        }
        if (strcmp(className, "Sse2") == 0)
        {
            return InstructionSet_SSE2;
        }
        if (strcmp(className, "Sse3") == 0)
        {
            return InstructionSet_SSE3;
        }
        if (strcmp(className, "Ssse3") == 0)
        {
            return InstructionSet_SSSE3;
        }
        if (strcmp(className, "Sse41") == 0)
        {
            return InstructionSet_SSE41;
        }
        if (strcmp(className, "Sse42") == 0)
        {
            return InstructionSet_SSE42;
        }
    }
    else if (className[0] == 'B')
    {
        if (strcmp(className, "Bmi1") == 0)
        {
            return InstructionSet_BMI1;
        }
        if (strcmp(className, "Bmi2") == 0)
        {
            return InstructionSet_BMI2;
        }
    }
    else if (className[0] == 'P')
    {
        if (strcmp(className, "Pclmulqdq") == 0)
        {
            return InstructionSet_PCLMULQDQ;
        }
        if (strcmp(className, "Popcnt") == 0)
        {
            return InstructionSet_POPCNT;
        }
    }
    else if (className[0] == 'V')
    {
        if (strncmp(className, "Vector128", 9) == 0)
        {
            return InstructionSet_Vector128;
        }
        else if (strncmp(className, "Vector256", 9) == 0)
        {
            return InstructionSet_Vector256;
        }
        else if (strncmp(className, "Vector512", 9) == 0)
        {
            return InstructionSet_Vector512;
        }
        else if (strcmp(className, "VL") == 0)
        {
            assert(!"VL.X64 support doesn't exist in the managed libraries and so is not yet implemented");
            return InstructionSet_ILLEGAL;
        }
    }
    else if (strcmp(className, "Fma") == 0)
    {
        return InstructionSet_FMA;
    }
    else if (strcmp(className, "Lzcnt") == 0)
    {
        return InstructionSet_LZCNT;
    }
    else if (strcmp(className, "X86Base") == 0)
    {
        return InstructionSet_X86Base;
    }
    else if (strcmp(className, "X86Serialize") == 0)
    {
        return InstructionSet_X86Serialize;
    }

    return InstructionSet_ILLEGAL;
}

//------------------------------------------------------------------------
// lookupIsa: Gets the InstructionSet for a given class name and enclosing class name
//
// Arguments:
//    className -- The name of the class associated with the InstructionSet to lookup
//    enclosingClassName -- The name of the enclosing class of X64 classes
//
// Return Value:
//    The InstructionSet associated with className and enclosingClassName
CORINFO_InstructionSet HWIntrinsicInfo::lookupIsa(const char* className, const char* enclosingClassName)
{
    assert(className != nullptr);

    if (strcmp(className, "X64") == 0)
    {
        assert(enclosingClassName != nullptr);
        return X64VersionOfIsa(lookupInstructionSet(enclosingClassName));
    }
    else if (strcmp(className, "VL") == 0)
    {
        assert(enclosingClassName != nullptr);
        return VLVersionOfIsa(lookupInstructionSet(enclosingClassName));
    }
    else
    {
        return lookupInstructionSet(className);
    }
}

//------------------------------------------------------------------------
// lookupImmUpperBound: Gets the upper bound for the imm-value of a given NamedIntrinsic
//
// Arguments:
//    id -- The NamedIntrinsic associated with the HWIntrinsic to lookup
//
// Return Value:
//     The upper bound for the imm-value of the intrinsic associated with id
//
int HWIntrinsicInfo::lookupImmUpperBound(NamedIntrinsic id)
{
    assert(HWIntrinsicInfo::lookupCategory(id) == HW_Category_IMM);

    switch (id)
    {
        case NI_AVX_Compare:
        case NI_AVX_CompareScalar:
        {
            assert(!HWIntrinsicInfo::HasFullRangeImm(id));
            return 31; // enum FloatComparisonMode has 32 values
        }

        case NI_AVX2_GatherVector128:
        case NI_AVX2_GatherVector256:
        case NI_AVX2_GatherMaskVector128:
        case NI_AVX2_GatherMaskVector256:
            return 8;

        default:
        {
            assert(HWIntrinsicInfo::HasFullRangeImm(id));
            return 255;
        }
    }
}

//------------------------------------------------------------------------
// isAVX2GatherIntrinsic: Check if the intrinsic is AVX Gather*
//
// Arguments:
//    id   -- The NamedIntrinsic associated with the HWIntrinsic to lookup
//
// Return Value:
//     true if id is AVX Gather* intrinsic
//
bool HWIntrinsicInfo::isAVX2GatherIntrinsic(NamedIntrinsic id)
{
    switch (id)
    {
        case NI_AVX2_GatherVector128:
        case NI_AVX2_GatherVector256:
        case NI_AVX2_GatherMaskVector128:
        case NI_AVX2_GatherMaskVector256:
            return true;
        default:
            return false;
    }
}

//------------------------------------------------------------------------
// lookupFloatComparisonModeForSwappedArgs: Get the floating-point comparison
//      mode to use when the operands are swapped.
//
// Arguments:
//    comparison -- The comparison mode used for (op1, op2)
//
// Return Value:
//     The comparison mode to use for (op2, op1)
//
FloatComparisonMode HWIntrinsicInfo::lookupFloatComparisonModeForSwappedArgs(FloatComparisonMode comparison)
{
    switch (comparison)
    {
        // These comparison modes are the same even if the operands are swapped

        case FloatComparisonMode::OrderedEqualNonSignaling:
            return FloatComparisonMode::OrderedEqualNonSignaling;
        case FloatComparisonMode::UnorderedNonSignaling:
            return FloatComparisonMode::UnorderedNonSignaling;
        case FloatComparisonMode::UnorderedNotEqualNonSignaling:
            return FloatComparisonMode::UnorderedNotEqualNonSignaling;
        case FloatComparisonMode::OrderedNonSignaling:
            return FloatComparisonMode::OrderedNonSignaling;
        case FloatComparisonMode::UnorderedEqualNonSignaling:
            return FloatComparisonMode::UnorderedEqualNonSignaling;
        case FloatComparisonMode::OrderedFalseNonSignaling:
            return FloatComparisonMode::OrderedFalseNonSignaling;
        case FloatComparisonMode::OrderedNotEqualNonSignaling:
            return FloatComparisonMode::OrderedNotEqualNonSignaling;
        case FloatComparisonMode::UnorderedTrueNonSignaling:
            return FloatComparisonMode::UnorderedTrueNonSignaling;
        case FloatComparisonMode::OrderedEqualSignaling:
            return FloatComparisonMode::OrderedEqualSignaling;
        case FloatComparisonMode::UnorderedSignaling:
            return FloatComparisonMode::UnorderedSignaling;
        case FloatComparisonMode::UnorderedNotEqualSignaling:
            return FloatComparisonMode::UnorderedNotEqualSignaling;
        case FloatComparisonMode::OrderedSignaling:
            return FloatComparisonMode::OrderedSignaling;
        case FloatComparisonMode::UnorderedEqualSignaling:
            return FloatComparisonMode::UnorderedEqualSignaling;
        case FloatComparisonMode::OrderedFalseSignaling:
            return FloatComparisonMode::OrderedFalseSignaling;
        case FloatComparisonMode::OrderedNotEqualSignaling:
            return FloatComparisonMode::OrderedNotEqualSignaling;
        case FloatComparisonMode::UnorderedTrueSignaling:
            return FloatComparisonMode::UnorderedTrueSignaling;

        // These comparison modes need a different mode if the operands are swapped

        case FloatComparisonMode::OrderedLessThanSignaling:
            return FloatComparisonMode::OrderedGreaterThanSignaling;
        case FloatComparisonMode::OrderedLessThanOrEqualSignaling:
            return FloatComparisonMode::OrderedGreaterThanOrEqualSignaling;
        case FloatComparisonMode::UnorderedNotLessThanSignaling:
            return FloatComparisonMode::UnorderedNotGreaterThanSignaling;
        case FloatComparisonMode::UnorderedNotLessThanOrEqualSignaling:
            return FloatComparisonMode::UnorderedNotGreaterThanOrEqualSignaling;
        case FloatComparisonMode::UnorderedNotGreaterThanOrEqualSignaling:
            return FloatComparisonMode::UnorderedNotLessThanOrEqualSignaling;
        case FloatComparisonMode::UnorderedNotGreaterThanSignaling:
            return FloatComparisonMode::UnorderedNotLessThanSignaling;
        case FloatComparisonMode::OrderedGreaterThanOrEqualSignaling:
            return FloatComparisonMode::OrderedLessThanOrEqualSignaling;
        case FloatComparisonMode::OrderedGreaterThanSignaling:
            return FloatComparisonMode::OrderedLessThanSignaling;
        case FloatComparisonMode::OrderedLessThanNonSignaling:
            return FloatComparisonMode::OrderedGreaterThanNonSignaling;
        case FloatComparisonMode::OrderedLessThanOrEqualNonSignaling:
            return FloatComparisonMode::OrderedGreaterThanOrEqualNonSignaling;
        case FloatComparisonMode::UnorderedNotLessThanNonSignaling:
            return FloatComparisonMode::UnorderedNotGreaterThanNonSignaling;
        case FloatComparisonMode::UnorderedNotLessThanOrEqualNonSignaling:
            return FloatComparisonMode::UnorderedNotGreaterThanOrEqualNonSignaling;
        case FloatComparisonMode::UnorderedNotGreaterThanOrEqualNonSignaling:
            return FloatComparisonMode::UnorderedNotLessThanOrEqualNonSignaling;
        case FloatComparisonMode::UnorderedNotGreaterThanNonSignaling:
            return FloatComparisonMode::UnorderedNotLessThanNonSignaling;
        case FloatComparisonMode::OrderedGreaterThanOrEqualNonSignaling:
            return FloatComparisonMode::OrderedLessThanOrEqualNonSignaling;
        case FloatComparisonMode::OrderedGreaterThanNonSignaling:
            return FloatComparisonMode::OrderedLessThanNonSignaling;

        default:
            unreached();
    }
}

//------------------------------------------------------------------------
// isFullyImplementedIsa: Gets a value that indicates whether the InstructionSet is fully implemented
//
// Arguments:
//    isa - The InstructionSet to check
//
// Return Value:
//    true if isa is supported; otherwise, false
bool HWIntrinsicInfo::isFullyImplementedIsa(CORINFO_InstructionSet isa)
{
    switch (isa)
    {
        // These ISAs are fully implemented
        case InstructionSet_AES:
        case InstructionSet_AES_X64:
        case InstructionSet_AVX:
        case InstructionSet_AVX_X64:
        case InstructionSet_AVX2:
        case InstructionSet_AVX2_X64:
        case InstructionSet_AVX512F:
        case InstructionSet_AVX512F_VL:
        case InstructionSet_AVX512F_VL_X64:
        case InstructionSet_AVX512F_X64:
        case InstructionSet_AVX512BW:
        case InstructionSet_AVX512BW_VL:
        case InstructionSet_AVX512BW_VL_X64:
        case InstructionSet_AVX512BW_X64:
        case InstructionSet_AVX512CD:
        case InstructionSet_AVX512CD_VL:
        case InstructionSet_AVX512CD_VL_X64:
        case InstructionSet_AVX512CD_X64:
        case InstructionSet_AVX512DQ:
        case InstructionSet_AVX512DQ_VL:
        case InstructionSet_AVX512DQ_VL_X64:
        case InstructionSet_AVX512DQ_X64:
        case InstructionSet_AVXVNNI:
        case InstructionSet_AVXVNNI_X64:
        case InstructionSet_BMI1:
        case InstructionSet_BMI1_X64:
        case InstructionSet_BMI2:
        case InstructionSet_BMI2_X64:
        case InstructionSet_FMA:
        case InstructionSet_FMA_X64:
        case InstructionSet_LZCNT:
        case InstructionSet_LZCNT_X64:
        case InstructionSet_PCLMULQDQ:
        case InstructionSet_PCLMULQDQ_X64:
        case InstructionSet_POPCNT:
        case InstructionSet_POPCNT_X64:
        case InstructionSet_SSE:
        case InstructionSet_SSE_X64:
        case InstructionSet_SSE2:
        case InstructionSet_SSE2_X64:
        case InstructionSet_SSE3:
        case InstructionSet_SSE3_X64:
        case InstructionSet_SSSE3:
        case InstructionSet_SSSE3_X64:
        case InstructionSet_SSE41:
        case InstructionSet_SSE41_X64:
        case InstructionSet_SSE42:
        case InstructionSet_SSE42_X64:
        case InstructionSet_Vector128:
        case InstructionSet_Vector256:
        case InstructionSet_Vector512:
        case InstructionSet_X86Base:
        case InstructionSet_X86Base_X64:
        case InstructionSet_X86Serialize:
        case InstructionSet_X86Serialize_X64:
        {
            return true;
        }

        default:
        {
            return false;
        }
    }
}

//------------------------------------------------------------------------
// isScalarIsa: Gets a value that indicates whether the InstructionSet is scalar
//
// Arguments:
//    isa - The InstructionSet to check
//
// Return Value:
//    true if isa is scalar; otherwise, false
bool HWIntrinsicInfo::isScalarIsa(CORINFO_InstructionSet isa)
{
    switch (isa)
    {
        case InstructionSet_BMI1:
        case InstructionSet_BMI1_X64:
        case InstructionSet_BMI2:
        case InstructionSet_BMI2_X64:
        case InstructionSet_LZCNT:
        case InstructionSet_LZCNT_X64:
        case InstructionSet_X86Base:
        case InstructionSet_X86Base_X64:
        {
            // InstructionSet_POPCNT and InstructionSet_POPCNT_X64 are excluded
            // even though they are "scalar" ISA because they depend on SSE4.2
            // and Popcnt.IsSupported implies Sse42.IsSupported
            return true;
        }

        default:
        {
            return false;
        }
    }
}

//------------------------------------------------------------------------
// impNonConstFallback: convert certain SSE2/AVX2 shift intrinsic to its semantic alternative when the imm-arg is
// not a compile-time constant
//
// Arguments:
//    intrinsic       -- intrinsic ID
//    simdType        -- Vector type
//    simdBaseJitType -- SIMD base JIT type of the Vector128/256<T>
//
// Return Value:
//     return the IR of semantic alternative on non-const imm-arg
//
GenTree* Compiler::impNonConstFallback(NamedIntrinsic intrinsic, var_types simdType, CorInfoType simdBaseJitType)
{
    assert(HWIntrinsicInfo::NoJmpTableImm(intrinsic));
    switch (intrinsic)
    {
        case NI_SSE2_ShiftLeftLogical:
        case NI_SSE2_ShiftRightArithmetic:
        case NI_SSE2_ShiftRightLogical:
        case NI_AVX2_ShiftLeftLogical:
        case NI_AVX2_ShiftRightArithmetic:
        case NI_AVX2_ShiftRightLogical:
        case NI_AVX512F_ShiftLeftLogical:
        case NI_AVX512F_ShiftRightArithmetic:
        case NI_AVX512F_ShiftRightLogical:
        case NI_AVX512F_VL_ShiftRightArithmetic:
        case NI_AVX512BW_ShiftLeftLogical:
        case NI_AVX512BW_ShiftRightArithmetic:
        case NI_AVX512BW_ShiftRightLogical:
        {
            impSpillSideEffect(true,
                               verCurrentState.esStackDepth - 2 DEBUGARG("Spilling op1 side effects for HWIntrinsic"));

            GenTree* op2 = impPopStack().val;
            GenTree* op1 = impSIMDPopStack(simdType);

            GenTree* tmpOp =
                gtNewSimdHWIntrinsicNode(TYP_SIMD16, op2, NI_SSE2_ConvertScalarToVector128Int32, CORINFO_TYPE_INT, 16);
            return gtNewSimdHWIntrinsicNode(simdType, op1, tmpOp, intrinsic, simdBaseJitType, genTypeSize(simdType));
        }

        default:
            return nullptr;
    }
}

//------------------------------------------------------------------------
// impSpecialIntrinsic: dispatch intrinsics to their own implementation
//
// Arguments:
//    intrinsic       -- id of the intrinsic function.
//    clsHnd          -- class handle containing the intrinsic function.
//    method          -- method handle of the intrinsic function.
//    sig             -- signature of the intrinsic call.
//    simdBaseJitType -- generic argument of the intrinsic.
//    retType         -- return type of the intrinsic.
// Return Value:
//    the expanded intrinsic.
//
GenTree* Compiler::impSpecialIntrinsic(NamedIntrinsic        intrinsic,
                                       CORINFO_CLASS_HANDLE  clsHnd,
                                       CORINFO_METHOD_HANDLE method,
                                       CORINFO_SIG_INFO*     sig,
                                       CorInfoType           simdBaseJitType,
                                       var_types             retType,
                                       unsigned              simdSize)
{
    GenTree* retNode = nullptr;
    GenTree* op1     = nullptr;
    GenTree* op2     = nullptr;
    GenTree* op3     = nullptr;
    GenTree* op4     = nullptr;

    CORINFO_InstructionSet isa = HWIntrinsicInfo::lookupIsa(intrinsic);

    var_types simdBaseType = TYP_UNKNOWN;
    if (simdSize != 0)
    {
        simdBaseType = JitType2PreciseVarType(simdBaseJitType);
        assert(varTypeIsArithmetic(simdBaseType));
    }

    switch (intrinsic)
    {
        case NI_Vector128_Abs:
        case NI_Vector256_Abs:
        {
            assert(sig->numArgs == 1);

            if ((simdSize != 32) || varTypeIsFloating(simdBaseType) || varTypeIsUnsigned(simdBaseType) ||
                compExactlyDependsOn(InstructionSet_AVX2))
            {
                op1     = impSIMDPopStack(retType);
                retNode = gtNewSimdAbsNode(retType, op1, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector128_Add:
        case NI_Vector256_Add:
        case NI_Vector512_Add:
        case NI_Vector128_op_Addition:
        case NI_Vector256_op_Addition:
        case NI_Vector512_op_Addition:
        {
            assert(sig->numArgs == 2);

            if ((simdSize != 32) || varTypeIsFloating(simdBaseType) || compExactlyDependsOn(InstructionSet_AVX2))
            {
                op2 = impSIMDPopStack(retType);
                op1 = impSIMDPopStack(retType);

                retNode = gtNewSimdBinOpNode(GT_ADD, retType, op1, op2, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector128_AndNot:
        case NI_Vector256_AndNot:
        case NI_Vector512_AndNot:
        {
            assert(sig->numArgs == 2);

            impSpillSideEffect(true,
                               verCurrentState.esStackDepth - 2 DEBUGARG("Spilling op1 side effects for HWIntrinsic"));

            op2 = impSIMDPopStack(retType);
            op1 = impSIMDPopStack(retType);

            retNode = gtNewSimdBinOpNode(GT_AND_NOT, retType, op1, op2, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector128_As:
        case NI_Vector128_AsByte:
        case NI_Vector128_AsDouble:
        case NI_Vector128_AsInt16:
        case NI_Vector128_AsInt32:
        case NI_Vector128_AsInt64:
        case NI_Vector128_AsNInt:
        case NI_Vector128_AsNUInt:
        case NI_Vector128_AsSByte:
        case NI_Vector128_AsSingle:
        case NI_Vector128_AsUInt16:
        case NI_Vector128_AsUInt32:
        case NI_Vector128_AsUInt64:
        case NI_Vector128_AsVector4:
        case NI_Vector256_As:
        case NI_Vector256_AsByte:
        case NI_Vector256_AsDouble:
        case NI_Vector256_AsInt16:
        case NI_Vector256_AsInt32:
        case NI_Vector256_AsInt64:
        case NI_Vector256_AsNInt:
        case NI_Vector256_AsNUInt:
        case NI_Vector256_AsSByte:
        case NI_Vector256_AsSingle:
        case NI_Vector256_AsUInt16:
        case NI_Vector256_AsUInt32:
        case NI_Vector256_AsUInt64:
        case NI_Vector512_As:
        case NI_Vector512_AsByte:
        case NI_Vector512_AsDouble:
        case NI_Vector512_AsInt16:
        case NI_Vector512_AsInt32:
        case NI_Vector512_AsInt64:
        case NI_Vector512_AsNInt:
        case NI_Vector512_AsNUInt:
        case NI_Vector512_AsSByte:
        case NI_Vector512_AsSingle:
        case NI_Vector512_AsUInt16:
        case NI_Vector512_AsUInt32:
        case NI_Vector512_AsUInt64:
        {
            // We fold away the cast here, as it only exists to satisfy
            // the type system. It is safe to do this here since the retNode type
            // and the signature return type are both the same TYP_SIMD.

            assert(sig->numArgs == 1);

            retNode = impSIMDPopStack(retType, /* expectAddr: */ false, sig->retTypeClass);
            SetOpLclRelatedToSIMDIntrinsic(retNode);
            assert(retNode->gtType == getSIMDTypeForSize(getSIMDTypeSizeInBytes(sig->retTypeSigClass)));
            break;
        }

        case NI_Vector128_AsVector:
        {
            assert(sig->numArgs == 1);

            if (getSIMDVectorRegisterByteLength() == YMM_REGSIZE_BYTES)
            {
                // Vector<T> is TYP_SIMD32, so we should treat this as a call to Vector128.ToVector256
                return impSpecialIntrinsic(NI_Vector128_ToVector256, clsHnd, method, sig, simdBaseJitType, retType,
                                           simdSize);
            }

            assert(getSIMDVectorRegisterByteLength() == XMM_REGSIZE_BYTES);

            // We fold away the cast here, as it only exists to satisfy
            // the type system. It is safe to do this here since the retNode type
            // and the signature return type are both the same TYP_SIMD.

            retNode = impSIMDPopStack(retType, /* expectAddr: */ false, sig->retTypeClass);
            SetOpLclRelatedToSIMDIntrinsic(retNode);
            assert(retNode->gtType == getSIMDTypeForSize(getSIMDTypeSizeInBytes(sig->retTypeSigClass)));

            break;
        }

        case NI_Vector128_AsVector2:
        case NI_Vector128_AsVector3:
        {
            assert(sig->numArgs == 1);
            assert((simdSize == 16) && (simdBaseType == TYP_FLOAT));
            assert((retType == TYP_SIMD8) || (retType == TYP_SIMD12));

            op1     = impSIMDPopStack(TYP_SIMD16);
            retNode = gtNewSimdHWIntrinsicNode(retType, op1, intrinsic, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector128_AsVector128:
        {
            assert(sig->numArgs == 1);
            assert(retType == TYP_SIMD16);
            assert(HWIntrinsicInfo::BaseTypeFromFirstArg(intrinsic));

            CorInfoType op1SimdBaseJitType =
                getBaseJitTypeAndSizeOfSIMDType(info.compCompHnd->getArgClass(sig, sig->args), &simdSize);

            assert(simdBaseJitType == op1SimdBaseJitType);

            switch (getSIMDTypeForSize(simdSize))
            {
                case TYP_SIMD8:
                {
                    assert((simdSize == 8) && (simdBaseType == TYP_FLOAT));

                    op1 = impSIMDPopStack(TYP_SIMD8);

                    if (op1->IsCnsVec())
                    {
                        GenTreeVecCon* vecCon = op1->AsVecCon();
                        vecCon->gtType        = TYP_SIMD16;

                        vecCon->gtSimdVal.f32[2] = 0.0f;
                        vecCon->gtSimdVal.f32[3] = 0.0f;

                        return vecCon;
                    }

                    GenTree* idx  = gtNewIconNode(2, TYP_INT);
                    GenTree* zero = gtNewZeroConNode(TYP_FLOAT);
                    op1           = gtNewSimdWithElementNode(retType, op1, idx, zero, simdBaseJitType, 16);

                    idx     = gtNewIconNode(3, TYP_INT);
                    zero    = gtNewZeroConNode(TYP_FLOAT);
                    retNode = gtNewSimdWithElementNode(retType, op1, idx, zero, simdBaseJitType, 16);

                    break;
                }

                case TYP_SIMD12:
                {
                    assert((simdSize == 12) && (simdBaseType == TYP_FLOAT));

                    op1 = impSIMDPopStack(TYP_SIMD12);

                    if (op1->IsCnsVec())
                    {
                        GenTreeVecCon* vecCon = op1->AsVecCon();
                        vecCon->gtType        = TYP_SIMD16;

                        vecCon->gtSimdVal.f32[3] = 0.0f;
                        return vecCon;
                    }

                    GenTree* idx  = gtNewIconNode(3, TYP_INT);
                    GenTree* zero = gtNewZeroConNode(TYP_FLOAT);
                    retNode       = gtNewSimdWithElementNode(retType, op1, idx, zero, simdBaseJitType, 16);
                    break;
                }

                case TYP_SIMD16:
                {
                    // We fold away the cast here, as it only exists to satisfy
                    // the type system. It is safe to do this here since the retNode type
                    // and the signature return type are both the same TYP_SIMD.

                    retNode = impSIMDPopStack(retType, /* expectAddr: */ false, sig->retTypeClass);
                    SetOpLclRelatedToSIMDIntrinsic(retNode);
                    assert(retNode->gtType == getSIMDTypeForSize(getSIMDTypeSizeInBytes(sig->retTypeSigClass)));

                    break;
                }

                case TYP_SIMD32:
                {
                    // Vector<T> is TYP_SIMD32, so we should treat this as a call to Vector256.GetLower
                    return impSpecialIntrinsic(NI_Vector256_GetLower, clsHnd, method, sig, simdBaseJitType, retType,
                                               simdSize);
                }

                default:
                {
                    unreached();
                }
            }

            break;
        }

        case NI_Vector256_AsVector:
        case NI_Vector256_AsVector256:
        {
            assert(sig->numArgs == 1);

            if (getSIMDVectorRegisterByteLength() == YMM_REGSIZE_BYTES)
            {
                // We fold away the cast here, as it only exists to satisfy
                // the type system. It is safe to do this here since the retNode type
                // and the signature return type are both the same TYP_SIMD.

                retNode = impSIMDPopStack(retType, /* expectAddr: */ false, sig->retTypeClass);
                SetOpLclRelatedToSIMDIntrinsic(retNode);
                assert(retNode->gtType == getSIMDTypeForSize(getSIMDTypeSizeInBytes(sig->retTypeSigClass)));

                break;
            }

            assert(getSIMDVectorRegisterByteLength() == XMM_REGSIZE_BYTES);

            if (compExactlyDependsOn(InstructionSet_AVX))
            {
                // We support Vector256 but Vector<T> is only 16-bytes, so we should
                // treat this method as a call to Vector256.GetLower or Vector128.ToVector256

                if (intrinsic == NI_Vector256_AsVector)
                {
                    return impSpecialIntrinsic(NI_Vector256_GetLower, clsHnd, method, sig, simdBaseJitType, retType,
                                               simdSize);
                }
                else
                {
                    assert(intrinsic == NI_Vector256_AsVector256);
                    return impSpecialIntrinsic(NI_Vector128_ToVector256, clsHnd, method, sig, simdBaseJitType, retType,
                                               16);
                }
            }

            break;
        }

        case NI_Vector512_AsVector:
        case NI_Vector512_AsVector512:
        {
            assert(sig->numArgs == 1);

            if (getSIMDVectorRegisterByteLength() == YMM_REGSIZE_BYTES)
            {
                assert(IsBaselineVector512IsaSupported());
                // We support Vector512 but Vector<T> is only 32-bytes, so we should
                // treat this method as a call to Vector512.GetLower or Vector256.ToVector512

                if (intrinsic == NI_Vector512_AsVector)
                {
                    return impSpecialIntrinsic(NI_Vector512_GetLower, clsHnd, method, sig, simdBaseJitType, retType,
                                               simdSize);
                }
                else
                {
                    assert(intrinsic == NI_Vector512_AsVector512);
                    return impSpecialIntrinsic(NI_Vector256_ToVector512, clsHnd, method, sig, simdBaseJitType, retType,
                                               32);
                }
                break;
            }

            assert(getSIMDVectorRegisterByteLength() == XMM_REGSIZE_BYTES);
            if (compExactlyDependsOn(InstructionSet_AVX512F))
            {
                // We support Vector512 but Vector<T> is only 16-bytes, so we should
                // treat this method as a call to Vector512.GetLower128 or Vector128.ToVector512

                if (intrinsic == NI_Vector512_AsVector)
                {
                    return impSpecialIntrinsic(NI_Vector512_GetLower128, clsHnd, method, sig, simdBaseJitType, retType,
                                               simdSize);
                }
                else
                {
                    assert(intrinsic == NI_Vector512_AsVector512);
                    return impSpecialIntrinsic(NI_Vector128_ToVector512, clsHnd, method, sig, simdBaseJitType, retType,
                                               16);
                }
            }
            break;
        }

        case NI_Vector128_BitwiseAnd:
        case NI_Vector256_BitwiseAnd:
        case NI_Vector512_BitwiseAnd:
        case NI_Vector128_op_BitwiseAnd:
        case NI_Vector256_op_BitwiseAnd:
        case NI_Vector512_op_BitwiseAnd:
        {
            assert(sig->numArgs == 2);

            op2 = impSIMDPopStack(retType);
            op1 = impSIMDPopStack(retType);

            retNode = gtNewSimdBinOpNode(GT_AND, retType, op1, op2, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector128_BitwiseOr:
        case NI_Vector256_BitwiseOr:
        case NI_Vector512_BitwiseOr:
        case NI_Vector128_op_BitwiseOr:
        case NI_Vector256_op_BitwiseOr:
        case NI_Vector512_op_BitwiseOr:
        {
            assert(sig->numArgs == 2);

            op2 = impSIMDPopStack(retType);
            op1 = impSIMDPopStack(retType);

            retNode = gtNewSimdBinOpNode(GT_OR, retType, op1, op2, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector128_Ceiling:
        case NI_Vector256_Ceiling:
        {
            assert(sig->numArgs == 1);
            assert(varTypeIsFloating(simdBaseType));

            if ((simdSize != 32) && !compExactlyDependsOn(InstructionSet_SSE41))
            {
                // Ceiling is only supported for floating-point types on SSE4.1 or later
                break;
            }

            op1     = impSIMDPopStack(retType);
            retNode = gtNewSimdCeilNode(retType, op1, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector128_ConditionalSelect:
        case NI_Vector256_ConditionalSelect:
        {
            assert(sig->numArgs == 3);

            op3 = impSIMDPopStack(retType);
            op2 = impSIMDPopStack(retType);
            op1 = impSIMDPopStack(retType);

            retNode = gtNewSimdCndSelNode(retType, op1, op2, op3, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector128_ConvertToDouble:
        case NI_Vector256_ConvertToDouble:
        case NI_Vector128_ConvertToInt64:
        case NI_Vector256_ConvertToInt64:
        case NI_Vector128_ConvertToUInt32:
        case NI_Vector256_ConvertToUInt32:
        case NI_Vector128_ConvertToUInt64:
        case NI_Vector256_ConvertToUInt64:
        {
            assert(sig->numArgs == 1);
            // TODO-XARCH-CQ: These intrinsics should be accelerated
            break;
        }

        case NI_Vector128_ConvertToInt32:
        case NI_Vector256_ConvertToInt32:
        {
            assert(sig->numArgs == 1);
            assert(simdBaseType == TYP_FLOAT);

            intrinsic = (simdSize == 32) ? NI_AVX_ConvertToVector256Int32WithTruncation
                                         : NI_SSE2_ConvertToVector128Int32WithTruncation;

            op1     = impSIMDPopStack(retType);
            retNode = gtNewSimdHWIntrinsicNode(retType, op1, intrinsic, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector128_ConvertToSingle:
        case NI_Vector256_ConvertToSingle:
        {
            assert(sig->numArgs == 1);

            if (simdBaseType == TYP_INT)
            {
                intrinsic = (simdSize == 32) ? NI_AVX_ConvertToVector256Single : NI_SSE2_ConvertToVector128Single;

                op1     = impSIMDPopStack(retType);
                retNode = gtNewSimdHWIntrinsicNode(retType, op1, intrinsic, simdBaseJitType, simdSize);
            }
            else
            {
                // TODO-XARCH-CQ: These intrinsics should be accelerated
                assert(simdBaseType == TYP_UINT);
            }
            break;
        }

        case NI_Vector128_Create:
        case NI_Vector256_Create:
        case NI_Vector512_Create:
        {
            if (sig->numArgs == 1)
            {
#if defined(TARGET_X86)
                if (varTypeIsLong(simdBaseType) && !impStackTop(0).val->IsIntegralConst())
                {
                    // TODO-XARCH-CQ: It may be beneficial to emit the movq
                    // instruction, which takes a 64-bit memory address and
                    // works on 32-bit x86 systems.
                    break;
                }
#endif // TARGET_X86

                op1     = impPopStack().val;
                retNode = gtNewSimdCreateBroadcastNode(retType, op1, simdBaseJitType, simdSize);
                break;
            }

            uint32_t simdLength = getSIMDVectorLength(simdSize, simdBaseType);
            assert(sig->numArgs == simdLength);

            bool isConstant = true;

            if (varTypeIsFloating(simdBaseType))
            {
                for (uint32_t index = 0; index < sig->numArgs; index++)
                {
                    GenTree* arg = impStackTop(index).val;

                    if (!arg->IsCnsFltOrDbl())
                    {
                        isConstant = false;
                        break;
                    }
                }
            }
            else
            {
                assert(varTypeIsIntegral(simdBaseType));

                for (uint32_t index = 0; index < sig->numArgs; index++)
                {
                    GenTree* arg = impStackTop(index).val;

                    if (!arg->IsIntegralConst())
                    {
                        isConstant = false;
                        break;
                    }
                }
            }

            if (isConstant)
            {
                // Some of the below code assumes 16/32/64 byte SIMD types
                assert((simdSize == 16) || (simdSize == 32) || (simdSize == 64));

                GenTreeVecCon* vecCon = gtNewVconNode(retType);

                switch (simdBaseType)
                {
                    case TYP_BYTE:
                    case TYP_UBYTE:
                    {
                        uint8_t cnsVal = 0;

                        for (uint32_t index = 0; index < sig->numArgs; index++)
                        {
                            cnsVal = static_cast<uint8_t>(impPopStack().val->AsIntConCommon()->IntegralValue());
                            vecCon->gtSimdVal.u8[simdLength - 1 - index] = cnsVal;
                        }
                        break;
                    }

                    case TYP_SHORT:
                    case TYP_USHORT:
                    {
                        uint16_t cnsVal = 0;

                        for (uint32_t index = 0; index < sig->numArgs; index++)
                        {
                            cnsVal = static_cast<uint16_t>(impPopStack().val->AsIntConCommon()->IntegralValue());
                            vecCon->gtSimdVal.u16[simdLength - 1 - index] = cnsVal;
                        }
                        break;
                    }

                    case TYP_INT:
                    case TYP_UINT:
                    {
                        uint32_t cnsVal = 0;

                        for (uint32_t index = 0; index < sig->numArgs; index++)
                        {
                            cnsVal = static_cast<uint32_t>(impPopStack().val->AsIntConCommon()->IntegralValue());
                            vecCon->gtSimdVal.u32[simdLength - 1 - index] = cnsVal;
                        }
                        break;
                    }

                    case TYP_LONG:
                    case TYP_ULONG:
                    {
                        uint64_t cnsVal = 0;

                        for (uint32_t index = 0; index < sig->numArgs; index++)
                        {
                            cnsVal = static_cast<uint64_t>(impPopStack().val->AsIntConCommon()->IntegralValue());
                            vecCon->gtSimdVal.u64[simdLength - 1 - index] = cnsVal;
                        }
                        break;
                    }

                    case TYP_FLOAT:
                    {
                        float cnsVal = 0;

                        for (uint32_t index = 0; index < sig->numArgs; index++)
                        {
                            cnsVal = static_cast<float>(impPopStack().val->AsDblCon()->DconValue());
                            vecCon->gtSimdVal.f32[simdLength - 1 - index] = cnsVal;
                        }
                        break;
                    }

                    case TYP_DOUBLE:
                    {
                        double cnsVal = 0;

                        for (uint32_t index = 0; index < sig->numArgs; index++)
                        {
                            double cnsVal = static_cast<double>(impPopStack().val->AsDblCon()->DconValue());
                            vecCon->gtSimdVal.f64[simdLength - 1 - index] = cnsVal;
                        }
                        break;
                    }

                    default:
                    {
                        unreached();
                    }
                }

                retNode = vecCon;
                break;
            }

#if defined(TARGET_X86)
            if (varTypeIsLong(simdBaseType))
            {
                // TODO-XARCH-CQ: It may be beneficial to emit the movq
                // instruction, which takes a 64-bit memory address and
                // works on 32-bit x86 systems.
                break;
            }
#endif // TARGET_X86

            IntrinsicNodeBuilder nodeBuilder(getAllocator(CMK_ASTNode), sig->numArgs);

            // TODO-CQ: We don't handle contiguous args for anything except TYP_FLOAT today

            GenTree* prevArg           = nullptr;
            bool     areArgsContiguous = (simdBaseType == TYP_FLOAT);

            for (int i = sig->numArgs - 1; i >= 0; i--)
            {
                GenTree* arg = impPopStack().val;

                if (areArgsContiguous)
                {
                    if (prevArg != nullptr)
                    {
                        // Recall that we are popping the args off the stack in reverse order.
                        areArgsContiguous = areArgumentsContiguous(arg, prevArg);
                    }

                    prevArg = arg;
                }

                nodeBuilder.AddOperand(i, arg);
            }

            if (areArgsContiguous)
            {
                op1                 = nodeBuilder.GetOperand(0);
                GenTree* op1Address = CreateAddressNodeForSimdHWIntrinsicCreate(op1, simdBaseType, simdSize);
                retNode             = gtNewOperNode(GT_IND, retType, op1Address);
            }
            else
            {
                retNode =
                    gtNewSimdHWIntrinsicNode(retType, std::move(nodeBuilder), intrinsic, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector128_CreateScalar:
        case NI_Vector256_CreateScalar:
        case NI_Vector512_CreateScalar:
        {
            assert(sig->numArgs == 1);

#if defined(TARGET_X86)
            if (varTypeIsLong(simdBaseType) && !impStackTop(0).val->IsIntegralConst())
            {
                // TODO-XARCH-CQ: It may be beneficial to emit the movq
                // instruction, which takes a 64-bit memory address and
                // works on 32-bit x86 systems.
                break;
            }
#endif // TARGET_X86

            op1     = impPopStack().val;
            retNode = gtNewSimdCreateScalarNode(retType, op1, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector128_CreateScalarUnsafe:
        case NI_Vector256_CreateScalarUnsafe:
        case NI_Vector512_CreateScalarUnsafe:
        {
            assert(sig->numArgs == 1);

#if defined(TARGET_X86)
            if (varTypeIsLong(simdBaseType) && !impStackTop(0).val->IsIntegralConst())
            {
                // TODO-XARCH-CQ: It may be beneficial to emit the movq
                // instruction, which takes a 64-bit memory address and
                // works on 32-bit x86 systems.
                break;
            }
#endif // TARGET_X86

            op1     = impPopStack().val;
            retNode = gtNewSimdCreateScalarUnsafeNode(retType, op1, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector128_Divide:
        case NI_Vector256_Divide:
        case NI_Vector128_op_Division:
        case NI_Vector256_op_Division:
        {
            assert(sig->numArgs == 2);

            if (!varTypeIsFloating(simdBaseType))
            {
                // We can't trivially handle division for integral types using SIMD
                break;
            }

            CORINFO_ARG_LIST_HANDLE arg1     = sig->args;
            CORINFO_ARG_LIST_HANDLE arg2     = info.compCompHnd->getArgNext(arg1);
            var_types               argType  = TYP_UNKNOWN;
            CORINFO_CLASS_HANDLE    argClass = NO_CLASS_HANDLE;

            argType = JITtype2varType(strip(info.compCompHnd->getArgType(sig, arg2, &argClass)));
            op2     = getArgForHWIntrinsic(argType, argClass);

            argType = JITtype2varType(strip(info.compCompHnd->getArgType(sig, arg1, &argClass)));
            op1     = getArgForHWIntrinsic(argType, argClass);

            retNode = gtNewSimdBinOpNode(GT_DIV, retType, op1, op2, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector128_Dot:
        case NI_Vector256_Dot:
        {
            assert(sig->numArgs == 2);
            var_types simdType = getSIMDTypeForSize(simdSize);

            if (varTypeIsByte(simdBaseType) || varTypeIsLong(simdBaseType))
            {
                // TODO-XARCH-CQ: We could support dot product for 8-bit and
                // 64-bit integers if we support multiplication for the same
                break;
            }

            if (simdSize == 32)
            {
                if (!varTypeIsFloating(simdBaseType) && !compExactlyDependsOn(InstructionSet_AVX2))
                {
                    // We can't deal with TYP_SIMD32 for integral types if the compiler doesn't support AVX2
                    break;
                }
            }
            else if ((simdBaseType == TYP_INT) || (simdBaseType == TYP_UINT))
            {
                if (!compExactlyDependsOn(InstructionSet_SSE41))
                {
                    // TODO-XARCH-CQ: We can support 32-bit integers if we updating multiplication
                    // to be lowered rather than imported as the relevant operations.
                    break;
                }
            }

            op2 = impSIMDPopStack(simdType);
            op1 = impSIMDPopStack(simdType);

            retNode = gtNewSimdDotProdNode(retType, op1, op2, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector128_Equals:
        case NI_Vector256_Equals:
        case NI_Vector512_Equals:
        {
            assert(sig->numArgs == 2);

            if ((simdSize != 32) || varTypeIsFloating(simdBaseType) || compExactlyDependsOn(InstructionSet_AVX2))
            {
                op2 = impSIMDPopStack(retType);
                op1 = impSIMDPopStack(retType);

                retNode = gtNewSimdCmpOpNode(GT_EQ, retType, op1, op2, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector512_EqualsAll:
        case NI_Vector512_op_Equality:
        {
            assert(sig->numArgs == 2);
            assert(IsBaselineVector512IsaSupportedDebugOnly());

            var_types simdType = getSIMDTypeForSize(simdSize);

            op2 = impSIMDPopStack(simdType);
            op1 = impSIMDPopStack(simdType);

            retNode = gtNewSimdCmpOpAllNode(GT_EQ, retType, op1, op2, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector128_EqualsAll:
        case NI_Vector256_EqualsAll:
        case NI_Vector128_op_Equality:
        case NI_Vector256_op_Equality:
        {
            assert(sig->numArgs == 2);

            if ((simdSize != 32) || varTypeIsFloating(simdBaseType) || compExactlyDependsOn(InstructionSet_AVX2))
            {
                var_types simdType = getSIMDTypeForSize(simdSize);

                op2 = impSIMDPopStack(simdType);
                op1 = impSIMDPopStack(simdType);

                retNode = gtNewSimdCmpOpAllNode(GT_EQ, retType, op1, op2, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector512_EqualsAny:
        {
            assert(sig->numArgs == 2);
            assert(simdSize == 64);
            assert(IsBaselineVector512IsaSupportedDebugOnly());

            var_types simdType = getSIMDTypeForSize(simdSize);

            op2 = impSIMDPopStack(simdType);
            op1 = impSIMDPopStack(simdType);

            retNode = gtNewSimdCmpOpAnyNode(GT_EQ, retType, op1, op2, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector128_EqualsAny:
        case NI_Vector256_EqualsAny:
        {
            assert(sig->numArgs == 2);

            if ((simdSize != 32) || compExactlyDependsOn(InstructionSet_AVX2))
            {
                var_types simdType = getSIMDTypeForSize(simdSize);

                op2 = impSIMDPopStack(simdType);
                op1 = impSIMDPopStack(simdType);

                retNode = gtNewSimdCmpOpAnyNode(GT_EQ, retType, op1, op2, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector512_ExtractMostSignificantBits:
        {
#if defined(TARGET_X86)
            // TODO-XARCH-CQ: It may be beneficial to decompose this operation
            break;
#endif // TARGET_X86

            if (IsBaselineVector512IsaSupported())
            {
                var_types simdType = getSIMDTypeForSize(simdSize);

                op1 = impSIMDPopStack(simdType);

                retNode = gtNewSimdHWIntrinsicNode(retType, op1, NI_AVX512F_MoveMaskSpecial, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector128_ExtractMostSignificantBits:
        case NI_Vector256_ExtractMostSignificantBits:
        {
            assert(sig->numArgs == 1);

            if ((simdSize != 32) || varTypeIsFloating(simdBaseType) || compExactlyDependsOn(InstructionSet_AVX2))
            {
                var_types simdType = getSIMDTypeForSize(simdSize);

                NamedIntrinsic moveMaskIntrinsic = NI_Illegal;
                NamedIntrinsic shuffleIntrinsic  = NI_Illegal;

                switch (simdBaseType)
                {
                    case TYP_BYTE:
                    case TYP_UBYTE:
                    {
                        op1               = impSIMDPopStack(simdType);
                        moveMaskIntrinsic = (simdSize == 32) ? NI_AVX2_MoveMask : NI_SSE2_MoveMask;
                        break;
                    }

                    case TYP_SHORT:
                    case TYP_USHORT:
                    {
                        simd_t simdVal = {};

                        assert((simdSize == 16) || (simdSize == 32) || (simdSize == 64));
                        simdBaseJitType = varTypeIsUnsigned(simdBaseType) ? CORINFO_TYPE_UBYTE : CORINFO_TYPE_BYTE;

                        // We want to tightly pack the most significant byte of each short/ushort
                        // and then zero the tightly packed least significant bytes
                        //
                        // The most significant bit being set means zero the value

                        simdVal.u64[0] = 0x0F0D0B0907050301;
                        simdVal.u64[1] = 0x8080808080808080;

                        if (simdSize == 32)
                        {
                            // Vector256 works on 2x128-bit lanes, so repeat the same indices for the upper lane

                            simdVal.u64[2] = 0x0F0D0B0907050301;
                            simdVal.u64[3] = 0x8080808080808080;

                            shuffleIntrinsic  = NI_AVX2_Shuffle;
                            moveMaskIntrinsic = NI_SSE2_MoveMask;
                        }
                        else if (compOpportunisticallyDependsOn(InstructionSet_SSSE3))
                        {
                            shuffleIntrinsic  = NI_SSSE3_Shuffle;
                            moveMaskIntrinsic = NI_SSE2_MoveMask;
                        }
                        else
                        {
                            return nullptr;
                        }

                        op2 = gtNewVconNode(simdType);
                        memcpy(&op2->AsVecCon()->gtSimdVal, &simdVal, simdSize);

                        op1 = impSIMDPopStack(simdType);
                        op1 = gtNewSimdHWIntrinsicNode(simdType, op1, op2, shuffleIntrinsic, simdBaseJitType, simdSize);

                        if (simdSize == 32)
                        {
                            CorInfoType simdOtherJitType;

                            // Since Vector256 is 2x128-bit lanes we need a full width permutation so we get the lower
                            // 64-bits of each lane next to eachother. The upper bits should be zero, but also don't
                            // matter so we can also then simplify down to a 128-bit move mask.

                            simdOtherJitType = (simdBaseType == TYP_UBYTE) ? CORINFO_TYPE_ULONG : CORINFO_TYPE_LONG;

                            op1 = gtNewSimdHWIntrinsicNode(simdType, op1, gtNewIconNode(0xD8), NI_AVX2_Permute4x64,
                                                           simdOtherJitType, simdSize);

                            simdType = TYP_SIMD16;

                            op1 = gtNewSimdGetLowerNode(simdType, op1, simdBaseJitType, simdSize);

                            simdSize = 16;
                        }
                        break;
                    }

                    case TYP_INT:
                    case TYP_UINT:
                    case TYP_FLOAT:
                    {
                        simdBaseJitType   = CORINFO_TYPE_FLOAT;
                        op1               = impSIMDPopStack(simdType);
                        moveMaskIntrinsic = (simdSize == 32) ? NI_AVX_MoveMask : NI_SSE_MoveMask;
                        break;
                    }

                    case TYP_LONG:
                    case TYP_ULONG:
                    case TYP_DOUBLE:
                    {
                        simdBaseJitType   = CORINFO_TYPE_DOUBLE;
                        op1               = impSIMDPopStack(simdType);
                        moveMaskIntrinsic = (simdSize == 32) ? NI_AVX_MoveMask : NI_SSE2_MoveMask;
                        break;
                    }

                    default:
                    {
                        unreached();
                    }
                }

                assert(moveMaskIntrinsic != NI_Illegal);
                assert(op1 != nullptr);

                retNode = gtNewSimdHWIntrinsicNode(retType, op1, moveMaskIntrinsic, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector128_Floor:
        case NI_Vector256_Floor:
        {
            assert(sig->numArgs == 1);
            assert(varTypeIsFloating(simdBaseType));

            if ((simdSize != 32) && !compExactlyDependsOn(InstructionSet_SSE41))
            {
                // Ceiling is only supported for floating-point types on SSE4.1 or later
                break;
            }

            op1     = impSIMDPopStack(retType);
            retNode = gtNewSimdFloorNode(retType, op1, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector128_get_AllBitsSet:
        case NI_Vector256_get_AllBitsSet:
        case NI_Vector512_get_AllBitsSet:
        {
            assert(sig->numArgs == 0);
            retNode = gtNewAllBitsSetConNode(retType);
            break;
        }

        case NI_Vector128_get_One:
        case NI_Vector256_get_One:
        case NI_Vector512_get_One:
        {
            assert(sig->numArgs == 0);
            retNode = gtNewOneConNode(retType, simdBaseType);
            break;
        }

        case NI_Vector128_get_Zero:
        case NI_Vector256_get_Zero:
        case NI_Vector512_get_Zero:
        {
            assert(sig->numArgs == 0);
            retNode = gtNewZeroConNode(retType);
            break;
        }

        case NI_Vector128_GetElement:
        case NI_Vector256_GetElement:
        case NI_Vector512_GetElement:
        {
            assert(sig->numArgs == 2);

            switch (simdBaseType)
            {
                // Using software fallback if simdBaseType is not supported by hardware
                case TYP_BYTE:
                case TYP_UBYTE:
                case TYP_INT:
                case TYP_UINT:
                case TYP_LONG:
                case TYP_ULONG:
                    if (!compExactlyDependsOn(InstructionSet_SSE41))
                    {
                        return nullptr;
                    }
                    break;

                case TYP_DOUBLE:
                case TYP_FLOAT:
                case TYP_SHORT:
                case TYP_USHORT:
                    // short/ushort/float/double is supported by SSE2
                    break;

                default:
                    unreached();
            }

            GenTree* op2 = impPopStack().val;
            GenTree* op1 = impSIMDPopStack(getSIMDTypeForSize(simdSize));

            retNode = gtNewSimdGetElementNode(retType, op1, op2, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector128_GreaterThan:
        case NI_Vector256_GreaterThan:
        {
            assert(sig->numArgs == 2);

            if ((simdSize != 32) || varTypeIsFloating(simdBaseType) || compExactlyDependsOn(InstructionSet_AVX2))
            {
                op2 = impSIMDPopStack(retType);
                op1 = impSIMDPopStack(retType);

                retNode = gtNewSimdCmpOpNode(GT_GT, retType, op1, op2, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector128_GreaterThanAll:
        case NI_Vector256_GreaterThanAll:
        {
            assert(sig->numArgs == 2);

            if ((simdSize != 32) || compExactlyDependsOn(InstructionSet_AVX2))
            {
                var_types simdType = getSIMDTypeForSize(simdSize);

                op2 = impSIMDPopStack(simdType);
                op1 = impSIMDPopStack(simdType);

                retNode = gtNewSimdCmpOpAllNode(GT_GT, retType, op1, op2, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector128_GreaterThanAny:
        case NI_Vector256_GreaterThanAny:
        {
            assert(sig->numArgs == 2);

            if ((simdSize != 32) || compExactlyDependsOn(InstructionSet_AVX2))
            {
                var_types simdType = getSIMDTypeForSize(simdSize);

                op2 = impSIMDPopStack(simdType);
                op1 = impSIMDPopStack(simdType);

                retNode = gtNewSimdCmpOpAnyNode(GT_GT, retType, op1, op2, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector128_GreaterThanOrEqual:
        case NI_Vector256_GreaterThanOrEqual:
        {
            assert(sig->numArgs == 2);

            if ((simdSize != 32) || varTypeIsFloating(simdBaseType) || compExactlyDependsOn(InstructionSet_AVX2))
            {
                op2 = impSIMDPopStack(retType);
                op1 = impSIMDPopStack(retType);

                retNode = gtNewSimdCmpOpNode(GT_GE, retType, op1, op2, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector128_GreaterThanOrEqualAll:
        case NI_Vector256_GreaterThanOrEqualAll:
        {
            assert(sig->numArgs == 2);

            if ((simdSize != 32) || compExactlyDependsOn(InstructionSet_AVX2))
            {
                var_types simdType = getSIMDTypeForSize(simdSize);

                op2 = impSIMDPopStack(simdType);
                op1 = impSIMDPopStack(simdType);

                retNode = gtNewSimdCmpOpAllNode(GT_GE, retType, op1, op2, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector128_GreaterThanOrEqualAny:
        case NI_Vector256_GreaterThanOrEqualAny:
        {
            assert(sig->numArgs == 2);

            if ((simdSize != 32) || compExactlyDependsOn(InstructionSet_AVX2))
            {
                var_types simdType = getSIMDTypeForSize(simdSize);

                op2 = impSIMDPopStack(simdType);
                op1 = impSIMDPopStack(simdType);

                retNode = gtNewSimdCmpOpAnyNode(GT_GE, retType, op1, op2, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector128_LessThan:
        case NI_Vector256_LessThan:
        {
            assert(sig->numArgs == 2);

            if ((simdSize != 32) || varTypeIsFloating(simdBaseType) || compExactlyDependsOn(InstructionSet_AVX2))
            {
                op2 = impSIMDPopStack(retType);
                op1 = impSIMDPopStack(retType);

                retNode = gtNewSimdCmpOpNode(GT_LT, retType, op1, op2, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector128_LessThanAll:
        case NI_Vector256_LessThanAll:
        {
            assert(sig->numArgs == 2);

            if ((simdSize != 32) || compExactlyDependsOn(InstructionSet_AVX2))
            {
                var_types simdType = getSIMDTypeForSize(simdSize);

                op2 = impSIMDPopStack(simdType);
                op1 = impSIMDPopStack(simdType);

                retNode = gtNewSimdCmpOpAllNode(GT_LT, retType, op1, op2, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector128_LessThanAny:
        case NI_Vector256_LessThanAny:
        {
            assert(sig->numArgs == 2);

            if ((simdSize != 32) || compExactlyDependsOn(InstructionSet_AVX2))
            {
                var_types simdType = getSIMDTypeForSize(simdSize);

                op2 = impSIMDPopStack(simdType);
                op1 = impSIMDPopStack(simdType);

                retNode = gtNewSimdCmpOpAnyNode(GT_LT, retType, op1, op2, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector128_LessThanOrEqual:
        case NI_Vector256_LessThanOrEqual:
        {
            assert(sig->numArgs == 2);

            if ((simdSize != 32) || varTypeIsFloating(simdBaseType) || compExactlyDependsOn(InstructionSet_AVX2))
            {
                op2 = impSIMDPopStack(retType);
                op1 = impSIMDPopStack(retType);

                retNode = gtNewSimdCmpOpNode(GT_LE, retType, op1, op2, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector128_LessThanOrEqualAll:
        case NI_Vector256_LessThanOrEqualAll:
        {
            assert(sig->numArgs == 2);

            if ((simdSize != 32) || compExactlyDependsOn(InstructionSet_AVX2))
            {
                var_types simdType = getSIMDTypeForSize(simdSize);

                op2 = impSIMDPopStack(simdType);
                op1 = impSIMDPopStack(simdType);

                retNode = gtNewSimdCmpOpAllNode(GT_LE, retType, op1, op2, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector128_LessThanOrEqualAny:
        case NI_Vector256_LessThanOrEqualAny:
        {
            assert(sig->numArgs == 2);

            if ((simdSize != 32) || compExactlyDependsOn(InstructionSet_AVX2))
            {
                var_types simdType = getSIMDTypeForSize(simdSize);

                op2 = impSIMDPopStack(simdType);
                op1 = impSIMDPopStack(simdType);

                retNode = gtNewSimdCmpOpAnyNode(GT_LE, retType, op1, op2, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_SSE_LoadVector128:
        case NI_SSE2_LoadVector128:
        case NI_AVX_LoadVector256:
        case NI_AVX512F_LoadVector512:
        case NI_AVX512BW_LoadVector512:
        case NI_Vector128_Load:
        case NI_Vector256_Load:
        case NI_Vector512_Load:
        case NI_Vector128_LoadUnsafe:
        case NI_Vector256_LoadUnsafe:
        case NI_Vector512_LoadUnsafe:
        {
            if (sig->numArgs == 2)
            {
                op2 = impPopStack().val;
            }
            else
            {
                assert(sig->numArgs == 1);
            }

            op1 = impPopStack().val;

            if (op1->OperIs(GT_CAST) && op1->gtGetOp1()->TypeIs(TYP_BYREF))
            {
                // If what we have is a BYREF, that's what we really want, so throw away the cast.
                op1 = op1->gtGetOp1();
            }

            if (sig->numArgs == 2)
            {
                op3 = gtNewIconNode(genTypeSize(simdBaseType), op2->TypeGet());
                op2 = gtNewOperNode(GT_MUL, op2->TypeGet(), op2, op3);
                op1 = gtNewOperNode(GT_ADD, op1->TypeGet(), op1, op2);
            }

            retNode = gtNewSimdLoadNode(retType, op1, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector128_LoadAligned:
        case NI_Vector256_LoadAligned:
        case NI_Vector512_LoadAligned:
        {
            assert(sig->numArgs == 1);

            op1 = impPopStack().val;

            if (op1->OperIs(GT_CAST) && op1->gtGetOp1()->TypeIs(TYP_BYREF))
            {
                // If what we have is a BYREF, that's what we really want, so throw away the cast.
                op1 = op1->gtGetOp1();
            }

            retNode = gtNewSimdLoadAlignedNode(retType, op1, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector128_LoadAlignedNonTemporal:
        case NI_Vector256_LoadAlignedNonTemporal:
        case NI_Vector512_LoadAlignedNonTemporal:
        {
            assert(sig->numArgs == 1);

            op1 = impPopStack().val;

            if (op1->OperIs(GT_CAST) && op1->gtGetOp1()->TypeIs(TYP_BYREF))
            {
                // If what we have is a BYREF, that's what we really want, so throw away the cast.
                op1 = op1->gtGetOp1();
            }

            retNode = gtNewSimdLoadNonTemporalNode(retType, op1, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector128_Max:
        case NI_Vector256_Max:
        {
            assert(sig->numArgs == 2);

            if ((simdSize != 32) || varTypeIsFloating(simdBaseType) || compExactlyDependsOn(InstructionSet_AVX2))
            {
                op2 = impSIMDPopStack(retType);
                op1 = impSIMDPopStack(retType);

                retNode = gtNewSimdMaxNode(retType, op1, op2, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector128_Min:
        case NI_Vector256_Min:
        {
            assert(sig->numArgs == 2);

            if ((simdSize != 32) || varTypeIsFloating(simdBaseType) || compExactlyDependsOn(InstructionSet_AVX2))
            {
                op2 = impSIMDPopStack(retType);
                op1 = impSIMDPopStack(retType);

                retNode = gtNewSimdMinNode(retType, op1, op2, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector128_Multiply:
        case NI_Vector256_Multiply:
        case NI_Vector128_op_Multiply:
        case NI_Vector256_op_Multiply:
        {
            assert(sig->numArgs == 2);

            if ((simdSize == 32) && !varTypeIsFloating(simdBaseType) && !compExactlyDependsOn(InstructionSet_AVX2))
            {
                // We can't deal with TYP_SIMD32 for integral types if the compiler doesn't support AVX2
                break;
            }

            if ((simdBaseType == TYP_BYTE) || (simdBaseType == TYP_UBYTE))
            {
                // TODO-XARCH-CQ: We should support byte/sbyte multiplication
                break;
            }

            if (varTypeIsLong(simdBaseType))
            {
                assert((simdSize == 16) || (simdSize == 32));

                if (!compOpportunisticallyDependsOn(InstructionSet_AVX512DQ_VL))
                {
                    // TODO-XARCH-CQ: We should support long/ulong multiplication
                    break;
                }

#if defined(TARGET_X86)
                // TODO-XARCH-CQ: We need to support 64-bit CreateBroadcast
                break;
#endif // TARGET_X86
            }

            CORINFO_ARG_LIST_HANDLE arg1     = sig->args;
            CORINFO_ARG_LIST_HANDLE arg2     = info.compCompHnd->getArgNext(arg1);
            var_types               argType  = TYP_UNKNOWN;
            CORINFO_CLASS_HANDLE    argClass = NO_CLASS_HANDLE;

            argType = JITtype2varType(strip(info.compCompHnd->getArgType(sig, arg2, &argClass)));
            op2     = getArgForHWIntrinsic(argType, argClass);

            argType = JITtype2varType(strip(info.compCompHnd->getArgType(sig, arg1, &argClass)));
            op1     = getArgForHWIntrinsic(argType, argClass);

            retNode = gtNewSimdBinOpNode(GT_MUL, retType, op1, op2, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector128_Narrow:
        case NI_Vector256_Narrow:
        case NI_Vector512_Narrow:
        {
            assert(sig->numArgs == 2);

            if ((simdSize != 32) || varTypeIsFloating(simdBaseType) || compExactlyDependsOn(InstructionSet_AVX2))
            {
                assert((simdSize != 64) || IsBaselineVector512IsaSupportedDebugOnly());

                op2 = impSIMDPopStack(retType);
                op1 = impSIMDPopStack(retType);

                retNode = gtNewSimdNarrowNode(retType, op1, op2, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector128_Negate:
        case NI_Vector256_Negate:
        case NI_Vector128_op_UnaryNegation:
        case NI_Vector256_op_UnaryNegation:
        {
            assert(sig->numArgs == 1);

            if ((simdSize != 32) || varTypeIsFloating(simdBaseType) || compExactlyDependsOn(InstructionSet_AVX2))
            {
                op1     = impSIMDPopStack(retType);
                retNode = gtNewSimdUnOpNode(GT_NEG, retType, op1, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector128_OnesComplement:
        case NI_Vector256_OnesComplement:
        case NI_Vector512_OnesComplement:
        case NI_Vector128_op_OnesComplement:
        case NI_Vector256_op_OnesComplement:
        case NI_Vector512_op_OnesComplement:
        {
            assert(sig->numArgs == 1);
            op1     = impSIMDPopStack(retType);
            retNode = gtNewSimdUnOpNode(GT_NOT, retType, op1, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector128_op_Inequality:
        case NI_Vector256_op_Inequality:
        {
            assert(sig->numArgs == 2);

            if ((simdSize != 32) || varTypeIsFloating(simdBaseType) || compExactlyDependsOn(InstructionSet_AVX2))
            {
                var_types simdType = getSIMDTypeForSize(simdSize);

                op2 = impSIMDPopStack(simdType);
                op1 = impSIMDPopStack(simdType);

                retNode = gtNewSimdCmpOpAnyNode(GT_NE, retType, op1, op2, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector512_op_Inequality:
        {
            assert(sig->numArgs == 2);

            if (IsBaselineVector512IsaSupported())
            {
                var_types simdType = getSIMDTypeForSize(simdSize);

                op2 = impSIMDPopStack(simdType);
                op1 = impSIMDPopStack(simdType);

                retNode = gtNewSimdCmpOpAnyNode(GT_NE, retType, op1, op2, simdBaseJitType, simdSize);
            }

            break;
        }

        case NI_Vector128_op_UnaryPlus:
        case NI_Vector256_op_UnaryPlus:
        {
            assert(sig->numArgs == 1);
            retNode = impSIMDPopStack(retType);
            break;
        }

        case NI_Vector128_Subtract:
        case NI_Vector256_Subtract:
        case NI_Vector512_Subtract:
        case NI_Vector128_op_Subtraction:
        case NI_Vector256_op_Subtraction:
        case NI_Vector512_op_Subtraction:
        {
            assert(sig->numArgs == 2);

            if ((simdSize != 32) || varTypeIsFloating(simdBaseType) || compExactlyDependsOn(InstructionSet_AVX2))
            {
                op2 = impSIMDPopStack(retType);
                op1 = impSIMDPopStack(retType);

                retNode = gtNewSimdBinOpNode(GT_SUB, retType, op1, op2, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector128_ShiftLeft:
        case NI_Vector256_ShiftLeft:
        case NI_Vector128_op_LeftShift:
        case NI_Vector256_op_LeftShift:
        {
            assert(sig->numArgs == 2);

            if (varTypeIsByte(simdBaseType))
            {
                // byte and sbyte would require more work to support
                break;
            }

            if ((simdSize != 32) || compExactlyDependsOn(InstructionSet_AVX2))
            {
                op2 = impPopStack().val;
                op1 = impSIMDPopStack(retType);

                retNode = gtNewSimdBinOpNode(GT_LSH, retType, op1, op2, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector128_ShiftRightArithmetic:
        case NI_Vector256_ShiftRightArithmetic:
        case NI_Vector128_op_RightShift:
        case NI_Vector256_op_RightShift:
        {
            assert(sig->numArgs == 2);

            if (varTypeIsByte(simdBaseType))
            {
                // byte and sbyte would require more work to support
                break;
            }

            if (varTypeIsLong(simdBaseType) || (simdBaseType == TYP_DOUBLE))
            {
                if (!compOpportunisticallyDependsOn(InstructionSet_AVX512F_VL))
                {
                    // long, ulong, and double would require more work to support
                    break;
                }
            }

            if ((simdSize != 32) || compExactlyDependsOn(InstructionSet_AVX2))
            {
                genTreeOps op = varTypeIsUnsigned(simdBaseType) ? GT_RSZ : GT_RSH;

                op2 = impPopStack().val;
                op1 = impSIMDPopStack(retType);

                retNode = gtNewSimdBinOpNode(op, retType, op1, op2, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector128_ShiftRightLogical:
        case NI_Vector256_ShiftRightLogical:
        case NI_Vector128_op_UnsignedRightShift:
        case NI_Vector256_op_UnsignedRightShift:
        {
            assert(sig->numArgs == 2);

            if (varTypeIsByte(simdBaseType))
            {
                // byte and sbyte would require more work to support
                break;
            }

            if ((simdSize != 32) || compExactlyDependsOn(InstructionSet_AVX2))
            {
                op2 = impPopStack().val;
                op1 = impSIMDPopStack(retType);

                retNode = gtNewSimdBinOpNode(GT_RSZ, retType, op1, op2, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector128_Shuffle:
        case NI_Vector256_Shuffle:
        {
            assert((sig->numArgs == 2) || (sig->numArgs == 3));
            assert((simdSize == 16) || (simdSize == 32));

            GenTree* indices = impStackTop(0).val;

            if (!indices->IsVectorConst())
            {
                // TODO-XARCH-CQ: Handling non-constant indices is a bit more complex
                break;
            }

            size_t elementSize  = genTypeSize(simdBaseType);
            size_t elementCount = simdSize / elementSize;

            if (simdSize == 32)
            {
                if (!compExactlyDependsOn(InstructionSet_AVX2))
                {
                    // While we could accelerate some functions on hardware with only AVX support
                    // it's likely not worth it overall given that IsHardwareAccelerated reports false
                    break;
                }
                else if (varTypeIsSmallInt(simdBaseType))
                {
                    bool crossLane = false;

                    for (size_t index = 0; index < elementCount; index++)
                    {
                        uint64_t value = indices->GetIntegralVectorConstElement(index, simdBaseType);

                        if (value >= elementCount)
                        {
                            continue;
                        }

                        if (index < (elementCount / 2))
                        {
                            if (value >= (elementCount / 2))
                            {
                                crossLane = true;
                                break;
                            }
                        }
                        else if (value < (elementCount / 2))
                        {
                            crossLane = true;
                            break;
                        }
                    }

                    if (crossLane)
                    {
                        // TODO-XARCH-CQ: We should emulate cross-lane shuffling for byte/sbyte and short/ushort
                        break;
                    }
                }
            }
            else
            {
                assert(simdSize == 16);

                if (varTypeIsSmallInt(simdBaseType) && !compExactlyDependsOn(InstructionSet_SSSE3))
                {
                    // TYP_BYTE, TYP_UBYTE, TYP_SHORT, and TYP_USHORT need SSSE3 to be able to shuffle any operation
                    break;
                }
            }

            if (sig->numArgs == 2)
            {
                op2 = impSIMDPopStack(retType);
                op1 = impSIMDPopStack(retType);

                retNode = gtNewSimdShuffleNode(retType, op1, op2, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector128_Sqrt:
        case NI_Vector256_Sqrt:
        {
            assert(sig->numArgs == 1);

            if (varTypeIsFloating(simdBaseType))
            {
                op1     = impSIMDPopStack(retType);
                retNode = gtNewSimdSqrtNode(retType, op1, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_SSE_Store:
        case NI_SSE2_Store:
        case NI_AVX_Store:
        case NI_AVX512F_Store:
        case NI_AVX512BW_Store:
        {
            assert(retType == TYP_VOID);
            assert(sig->numArgs == 2);

            var_types simdType = getSIMDTypeForSize(simdSize);

            op2 = impSIMDPopStack(simdType);
            op1 = impPopStack().val;

            if (op1->OperIs(GT_CAST) && op1->gtGetOp1()->TypeIs(TYP_BYREF))
            {
                // If what we have is a BYREF, that's what we really want, so throw away the cast.
                op1 = op1->gtGetOp1();
            }

            retNode = gtNewSimdStoreNode(op1, op2, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector128_Store:
        case NI_Vector256_Store:
        case NI_Vector512_Store:
        case NI_Vector128_StoreUnsafe:
        case NI_Vector256_StoreUnsafe:
        case NI_Vector512_StoreUnsafe:
        {
            assert(retType == TYP_VOID);
            var_types simdType = getSIMDTypeForSize(simdSize);

            if (sig->numArgs == 3)
            {
                impSpillSideEffect(true, verCurrentState.esStackDepth -
                                             3 DEBUGARG("Spilling op1 side effects for HWIntrinsic"));

                op3 = impPopStack().val;
            }
            else
            {
                assert(sig->numArgs == 2);

                impSpillSideEffect(true, verCurrentState.esStackDepth -
                                             2 DEBUGARG("Spilling op1 side effects for HWIntrinsic"));
            }

            op2 = impPopStack().val;

            if (op2->OperIs(GT_CAST) && op2->gtGetOp1()->TypeIs(TYP_BYREF))
            {
                // If what we have is a BYREF, that's what we really want, so throw away the cast.
                op2 = op2->gtGetOp1();
            }

            if (sig->numArgs == 3)
            {
                op4 = gtNewIconNode(genTypeSize(simdBaseType), op3->TypeGet());
                op3 = gtNewOperNode(GT_MUL, op3->TypeGet(), op3, op4);
                op2 = gtNewOperNode(GT_ADD, op2->TypeGet(), op2, op3);
            }

            op1 = impSIMDPopStack(simdType);

            retNode = gtNewSimdStoreNode(op2, op1, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector128_StoreAligned:
        case NI_Vector256_StoreAligned:
        case NI_Vector512_StoreAligned:
        {
            assert(sig->numArgs == 2);
            assert(retType == TYP_VOID);

            var_types simdType = getSIMDTypeForSize(simdSize);

            impSpillSideEffect(true,
                               verCurrentState.esStackDepth - 2 DEBUGARG("Spilling op1 side effects for HWIntrinsic"));

            op2 = impPopStack().val;

            if (op2->OperIs(GT_CAST) && op2->gtGetOp1()->TypeIs(TYP_BYREF))
            {
                // If what we have is a BYREF, that's what we really want, so throw away the cast.
                op2 = op2->gtGetOp1();
            }

            op1 = impSIMDPopStack(simdType);

            retNode = gtNewSimdStoreAlignedNode(op2, op1, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector128_StoreAlignedNonTemporal:
        case NI_Vector256_StoreAlignedNonTemporal:
        case NI_Vector512_StoreAlignedNonTemporal:
        {
            assert(sig->numArgs == 2);
            assert(retType == TYP_VOID);

            var_types simdType = getSIMDTypeForSize(simdSize);

            impSpillSideEffect(true,
                               verCurrentState.esStackDepth - 2 DEBUGARG("Spilling op1 side effects for HWIntrinsic"));

            op2 = impPopStack().val;

            if (op2->OperIs(GT_CAST) && op2->gtGetOp1()->TypeIs(TYP_BYREF))
            {
                // If what we have is a BYREF, that's what we really want, so throw away the cast.
                op2 = op2->gtGetOp1();
            }

            op1 = impSIMDPopStack(simdType);

            retNode = gtNewSimdStoreNonTemporalNode(op2, op1, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector128_Sum:
        case NI_Vector256_Sum:
        {
            assert(sig->numArgs == 1);
            var_types simdType = getSIMDTypeForSize(simdSize);

            if ((simdSize == 32) && !compOpportunisticallyDependsOn(InstructionSet_AVX2))
            {
                // Vector256 for integer types requires AVX2
                break;
            }
            else if (varTypeIsFloating(simdBaseType))
            {
                if (!compOpportunisticallyDependsOn(InstructionSet_SSE3))
                {
                    // Floating-point types require SSE3.HorizontalAdd
                    break;
                }
            }
            else if (!compOpportunisticallyDependsOn(InstructionSet_SSSE3))
            {
                // Integral types require SSSE3.HorizontalAdd
                break;
            }
            else if (varTypeIsByte(simdBaseType) || varTypeIsLong(simdBaseType))
            {
                // byte, sbyte, long, and ulong all would require more work to support
                break;
            }

            op1     = impSIMDPopStack(simdType);
            retNode = gtNewSimdSumNode(retType, op1, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector128_ToScalar:
        case NI_Vector256_ToScalar:
        case NI_Vector512_ToScalar:
        {
            assert(sig->numArgs == 1);

#if defined(TARGET_X86)
            if (varTypeIsLong(simdBaseType))
            {
                // TODO-XARCH-CQ: It may be beneficial to decompose this operation
                break;
            }
#endif // TARGET_X86

            // TODO-XARCH-CQ: It may be beneficial to import this as GetElement(0)
            op1     = impSIMDPopStack(getSIMDTypeForSize(simdSize));
            retNode = gtNewSimdHWIntrinsicNode(retType, op1, intrinsic, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector128_ToVector256:
        case NI_Vector128_ToVector256Unsafe:
        {
            assert(sig->numArgs == 1);
            assert(compIsaSupportedDebugOnly(InstructionSet_AVX));

            op1     = impSIMDPopStack(getSIMDTypeForSize(simdSize));
            retNode = gtNewSimdHWIntrinsicNode(retType, op1, intrinsic, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector256_GetLower:
        {
            assert(sig->numArgs == 1);
            assert(compIsaSupportedDebugOnly(InstructionSet_AVX));

            op1     = impSIMDPopStack(getSIMDTypeForSize(simdSize));
            retNode = gtNewSimdGetLowerNode(retType, op1, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector256_GetUpper:
        {
            assert(sig->numArgs == 1);
            assert(compIsaSupportedDebugOnly(InstructionSet_AVX));

            op1     = impSIMDPopStack(getSIMDTypeForSize(simdSize));
            retNode = gtNewSimdGetUpperNode(retType, op1, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector512_GetLower:
        {
            assert(sig->numArgs == 1);
            assert(IsBaselineVector512IsaSupportedDebugOnly());

            op1     = impSIMDPopStack(getSIMDTypeForSize(simdSize));
            retNode = gtNewSimdGetLowerNode(retType, op1, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector512_GetUpper:
        {
            assert(sig->numArgs == 1);
            assert(IsBaselineVector512IsaSupportedDebugOnly());

            op1     = impSIMDPopStack(getSIMDTypeForSize(simdSize));
            retNode = gtNewSimdGetUpperNode(retType, op1, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector128_ToVector512:
        case NI_Vector256_ToVector512:
        case NI_Vector256_ToVector512Unsafe:
        case NI_Vector512_GetLower128:
        {
            assert(sig->numArgs == 1);
            assert(IsBaselineVector512IsaSupportedDebugOnly());

            op1     = impSIMDPopStack(getSIMDTypeForSize(simdSize));
            retNode = gtNewSimdHWIntrinsicNode(retType, op1, intrinsic, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector128_WidenLower:
        case NI_Vector256_WidenLower:
        case NI_Vector512_WidenLower:
        {
            assert(sig->numArgs == 1);

            if ((simdSize != 32) || varTypeIsFloating(simdBaseType) || compExactlyDependsOn(InstructionSet_AVX2))
            {
                assert((simdSize != 64) || IsBaselineVector512IsaSupportedDebugOnly());

                op1 = impSIMDPopStack(retType);

                retNode = gtNewSimdWidenLowerNode(retType, op1, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector128_WidenUpper:
        case NI_Vector256_WidenUpper:
        case NI_Vector512_WidenUpper:
        {
            assert(sig->numArgs == 1);

            if ((simdSize != 32) || varTypeIsFloating(simdBaseType) || compExactlyDependsOn(InstructionSet_AVX2))
            {
                assert((simdSize != 64) || IsBaselineVector512IsaSupportedDebugOnly());

                op1 = impSIMDPopStack(retType);

                retNode = gtNewSimdWidenUpperNode(retType, op1, simdBaseJitType, simdSize);
            }
            break;
        }

        case NI_Vector128_WithElement:
        case NI_Vector256_WithElement:
        case NI_Vector512_WithElement:
        {
            assert(sig->numArgs == 3);
            GenTree* indexOp = impStackTop(1).val;

            if (!indexOp->OperIsConst())
            {
                // TODO-XARCH-CQ: We should always import these like we do with GetElement
                // Index is not a constant, use the software fallback
                return nullptr;
            }

            ssize_t imm8  = indexOp->AsIntCon()->IconValue();
            ssize_t count = simdSize / genTypeSize(simdBaseType);

            if ((imm8 >= count) || (imm8 < 0))
            {
                // Using software fallback if index is out of range (throw exception)
                return nullptr;
            }

            switch (simdBaseType)
            {
                // Using software fallback if simdBaseType is not supported by hardware
                case TYP_BYTE:
                case TYP_UBYTE:
                case TYP_INT:
                case TYP_UINT:
                    if (!compExactlyDependsOn(InstructionSet_SSE41))
                    {
                        return nullptr;
                    }
                    break;

                case TYP_LONG:
                case TYP_ULONG:
                    if (!compExactlyDependsOn(InstructionSet_SSE41_X64))
                    {
                        return nullptr;
                    }
                    break;

                case TYP_DOUBLE:
                case TYP_FLOAT:
                case TYP_SHORT:
                case TYP_USHORT:
                    // short/ushort/float/double is supported by SSE2
                    break;

                default:
                    unreached();
            }

            GenTree* valueOp = impPopStack().val;
            impPopStack(); // Pop the indexOp now that we know its valid
            GenTree* vectorOp = impSIMDPopStack(getSIMDTypeForSize(simdSize));

            retNode = gtNewSimdWithElementNode(retType, vectorOp, indexOp, valueOp, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector256_WithLower:
        {
            assert(sig->numArgs == 2);
            assert(compIsaSupportedDebugOnly(InstructionSet_AVX));

            op2     = impSIMDPopStack(TYP_SIMD16);
            op1     = impSIMDPopStack(TYP_SIMD32);
            retNode = gtNewSimdWithLowerNode(retType, op1, op2, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector256_WithUpper:
        {
            assert(sig->numArgs == 2);
            assert(compIsaSupportedDebugOnly(InstructionSet_AVX));

            op2     = impSIMDPopStack(TYP_SIMD16);
            op1     = impSIMDPopStack(TYP_SIMD32);
            retNode = gtNewSimdWithUpperNode(retType, op1, op2, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector512_WithLower:
        {
            assert(sig->numArgs == 2);
            assert(IsBaselineVector512IsaSupportedDebugOnly());

            op2     = impSIMDPopStack(TYP_SIMD32);
            op1     = impSIMDPopStack(TYP_SIMD64);
            retNode = gtNewSimdWithLowerNode(retType, op1, op2, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector512_WithUpper:
        {
            assert(sig->numArgs == 2);
            assert(IsBaselineVector512IsaSupportedDebugOnly());

            op2     = impSIMDPopStack(TYP_SIMD32);
            op1     = impSIMDPopStack(TYP_SIMD64);
            retNode = gtNewSimdWithUpperNode(retType, op1, op2, simdBaseJitType, simdSize);
            break;
        }

        case NI_Vector128_Xor:
        case NI_Vector256_Xor:
        case NI_Vector512_Xor:
        case NI_Vector128_op_ExclusiveOr:
        case NI_Vector256_op_ExclusiveOr:
        case NI_Vector512_op_ExclusiveOr:
        {
            assert(sig->numArgs == 2);

            op2 = impSIMDPopStack(retType);
            op1 = impSIMDPopStack(retType);

            retNode = gtNewSimdBinOpNode(GT_XOR, retType, op1, op2, simdBaseJitType, simdSize);
            break;
        }

        case NI_X86Base_Pause:
        case NI_X86Serialize_Serialize:
        {
            assert(sig->numArgs == 0);
            assert(JITtype2varType(sig->retType) == TYP_VOID);
            assert(simdSize == 0);

            retNode = gtNewScalarHWIntrinsicNode(TYP_VOID, intrinsic);
            break;
        }

        case NI_X86Base_DivRem:
        case NI_X86Base_X64_DivRem:
        {
            assert(sig->numArgs == 3);
            assert(HWIntrinsicInfo::IsMultiReg(intrinsic));
            assert(retType == TYP_STRUCT);
            assert(simdBaseJitType != CORINFO_TYPE_UNDEF);

            op3 = impPopStack().val;
            op2 = impPopStack().val;
            op1 = impPopStack().val;

            GenTreeHWIntrinsic* divRemIntrinsic = gtNewScalarHWIntrinsicNode(retType, op1, op2, op3, intrinsic);

            // Store the type from signature into SIMD base type for convenience
            divRemIntrinsic->SetSimdBaseJitType(simdBaseJitType);

            retNode = impAssignMultiRegTypeToVar(divRemIntrinsic,
                                                 sig->retTypeSigClass DEBUGARG(CorInfoCallConvExtension::Managed));
            break;
        }

        case NI_SSE_CompareScalarGreaterThan:
        case NI_SSE_CompareScalarGreaterThanOrEqual:
        case NI_SSE_CompareScalarNotGreaterThan:
        case NI_SSE_CompareScalarNotGreaterThanOrEqual:
        {
            assert(sig->numArgs == 2);

            bool supportsAvx = compOpportunisticallyDependsOn(InstructionSet_AVX);

            if (!supportsAvx)
            {
                impSpillSideEffect(true, verCurrentState.esStackDepth -
                                             2 DEBUGARG("Spilling op1 side effects for HWIntrinsic"));
            }

            op2             = impSIMDPopStack(TYP_SIMD16);
            op1             = impSIMDPopStack(TYP_SIMD16);
            simdBaseJitType = getBaseJitTypeOfSIMDType(sig->retTypeSigClass);
            assert(JitType2PreciseVarType(simdBaseJitType) == TYP_FLOAT);

            if (supportsAvx)
            {
                // These intrinsics are "special import" because the non-AVX path isn't directly
                // hardware supported. Instead, they start with "swapped operands" and we fix that here.

                FloatComparisonMode comparison =
                    static_cast<FloatComparisonMode>(HWIntrinsicInfo::lookupIval(intrinsic, true));
                retNode = gtNewSimdHWIntrinsicNode(TYP_SIMD16, op1, op2, gtNewIconNode(static_cast<int>(comparison)),
                                                   NI_AVX_CompareScalar, simdBaseJitType, simdSize);
            }
            else
            {
                GenTree* clonedOp1 = nullptr;
                op1                = impCloneExpr(op1, &clonedOp1, NO_CLASS_HANDLE, CHECK_SPILL_ALL,
                                   nullptr DEBUGARG("Clone op1 for Sse.CompareScalarGreaterThan"));

                retNode = gtNewSimdHWIntrinsicNode(TYP_SIMD16, op2, op1, intrinsic, simdBaseJitType, simdSize);
                retNode = gtNewSimdHWIntrinsicNode(TYP_SIMD16, clonedOp1, retNode, NI_SSE_MoveScalar, simdBaseJitType,
                                                   simdSize);
            }
            break;
        }

        case NI_SSE_Prefetch0:
        case NI_SSE_Prefetch1:
        case NI_SSE_Prefetch2:
        case NI_SSE_PrefetchNonTemporal:
        {
            assert(sig->numArgs == 1);
            assert(JITtype2varType(sig->retType) == TYP_VOID);
            op1     = impPopStack().val;
            retNode = gtNewSimdHWIntrinsicNode(TYP_VOID, op1, intrinsic, CORINFO_TYPE_UBYTE, 0);
            break;
        }

        case NI_SSE_StoreFence:
            assert(sig->numArgs == 0);
            assert(JITtype2varType(sig->retType) == TYP_VOID);
            retNode = gtNewScalarHWIntrinsicNode(TYP_VOID, intrinsic);
            break;

        case NI_SSE2_CompareScalarGreaterThan:
        case NI_SSE2_CompareScalarGreaterThanOrEqual:
        case NI_SSE2_CompareScalarNotGreaterThan:
        case NI_SSE2_CompareScalarNotGreaterThanOrEqual:
        {
            assert(sig->numArgs == 2);

            bool supportsAvx = compOpportunisticallyDependsOn(InstructionSet_AVX);

            if (!supportsAvx)
            {
                impSpillSideEffect(true, verCurrentState.esStackDepth -
                                             2 DEBUGARG("Spilling op1 side effects for HWIntrinsic"));
            }

            op2 = impSIMDPopStack(TYP_SIMD16);
            op1 = impSIMDPopStack(TYP_SIMD16);
            assert(JitType2PreciseVarType(simdBaseJitType) == TYP_DOUBLE);

            if (supportsAvx)
            {
                // These intrinsics are "special import" because the non-AVX path isn't directly
                // hardware supported. Instead, they start with "swapped operands" and we fix that here.

                FloatComparisonMode comparison =
                    static_cast<FloatComparisonMode>(HWIntrinsicInfo::lookupIval(intrinsic, true));
                retNode = gtNewSimdHWIntrinsicNode(TYP_SIMD16, op1, op2, gtNewIconNode(static_cast<int>(comparison)),
                                                   NI_AVX_CompareScalar, simdBaseJitType, simdSize);
            }
            else
            {
                GenTree* clonedOp1 = nullptr;
                op1                = impCloneExpr(op1, &clonedOp1, NO_CLASS_HANDLE, CHECK_SPILL_ALL,
                                   nullptr DEBUGARG("Clone op1 for Sse2.CompareScalarGreaterThan"));

                retNode = gtNewSimdHWIntrinsicNode(TYP_SIMD16, op2, op1, intrinsic, simdBaseJitType, simdSize);
                retNode = gtNewSimdHWIntrinsicNode(TYP_SIMD16, clonedOp1, retNode, NI_SSE2_MoveScalar, simdBaseJitType,
                                                   simdSize);
            }
            break;
        }

        case NI_SSE2_LoadFence:
        case NI_SSE2_MemoryFence:
        {
            assert(sig->numArgs == 0);
            assert(JITtype2varType(sig->retType) == TYP_VOID);
            assert(simdSize == 0);

            retNode = gtNewScalarHWIntrinsicNode(TYP_VOID, intrinsic);
            break;
        }

        case NI_SSE2_StoreNonTemporal:
        {
            assert(sig->numArgs == 2);
            assert(JITtype2varType(sig->retType) == TYP_VOID);

            CORINFO_ARG_LIST_HANDLE argList = info.compCompHnd->getArgNext(sig->args);
            CORINFO_CLASS_HANDLE    argClass;
            CorInfoType             argJitType = strip(info.compCompHnd->getArgType(sig, argList, &argClass));

            op2     = impPopStack().val;
            op1     = impPopStack().val;
            retNode = gtNewSimdHWIntrinsicNode(TYP_VOID, op1, op2, NI_SSE2_StoreNonTemporal, argJitType, 0);
            break;
        }

        case NI_AVX2_PermuteVar8x32:
        {
            simdBaseJitType = getBaseJitTypeOfSIMDType(sig->retTypeSigClass);

            impSpillSideEffect(true,
                               verCurrentState.esStackDepth - 2 DEBUGARG("Spilling op1 side effects for HWIntrinsic"));

            // swap the two operands
            GenTree* indexVector  = impSIMDPopStack(TYP_SIMD32);
            GenTree* sourceVector = impSIMDPopStack(TYP_SIMD32);

            retNode = gtNewSimdHWIntrinsicNode(TYP_SIMD32, indexVector, sourceVector, NI_AVX2_PermuteVar8x32,
                                               simdBaseJitType, 32);
            break;
        }

        case NI_AVX2_GatherMaskVector128:
        case NI_AVX2_GatherMaskVector256:
        {
            CORINFO_ARG_LIST_HANDLE argList = sig->args;
            CORINFO_CLASS_HANDLE    argClass;
            var_types               argType = TYP_UNKNOWN;
            unsigned int            sizeBytes;
            simdBaseJitType   = getBaseJitTypeAndSizeOfSIMDType(sig->retTypeSigClass, &sizeBytes);
            var_types retType = getSIMDTypeForSize(sizeBytes);

            assert(sig->numArgs == 5);
            CORINFO_ARG_LIST_HANDLE arg2 = info.compCompHnd->getArgNext(argList);
            CORINFO_ARG_LIST_HANDLE arg3 = info.compCompHnd->getArgNext(arg2);
            CORINFO_ARG_LIST_HANDLE arg4 = info.compCompHnd->getArgNext(arg3);
            CORINFO_ARG_LIST_HANDLE arg5 = info.compCompHnd->getArgNext(arg4);

            argType      = JITtype2varType(strip(info.compCompHnd->getArgType(sig, arg5, &argClass)));
            GenTree* op5 = getArgForHWIntrinsic(argType, argClass);
            SetOpLclRelatedToSIMDIntrinsic(op5);

            argType      = JITtype2varType(strip(info.compCompHnd->getArgType(sig, arg4, &argClass)));
            GenTree* op4 = getArgForHWIntrinsic(argType, argClass);
            SetOpLclRelatedToSIMDIntrinsic(op4);

            argType                      = JITtype2varType(strip(info.compCompHnd->getArgType(sig, arg3, &argClass)));
            CorInfoType indexBaseJitType = getBaseJitTypeOfSIMDType(argClass);
            GenTree*    op3              = getArgForHWIntrinsic(argType, argClass);
            SetOpLclRelatedToSIMDIntrinsic(op3);

            argType = JITtype2varType(strip(info.compCompHnd->getArgType(sig, arg2, &argClass)));
            op2     = getArgForHWIntrinsic(argType, argClass);
            SetOpLclRelatedToSIMDIntrinsic(op2);

            argType = JITtype2varType(strip(info.compCompHnd->getArgType(sig, argList, &argClass)));
            op1     = getArgForHWIntrinsic(argType, argClass);
            SetOpLclRelatedToSIMDIntrinsic(op1);

            retNode = new (this, GT_HWINTRINSIC) GenTreeHWIntrinsic(retType, getAllocator(CMK_ASTNode), intrinsic,
                                                                    simdBaseJitType, simdSize, op1, op2, op3, op4, op5);
            retNode->AsHWIntrinsic()->SetAuxiliaryJitType(indexBaseJitType);
            break;
        }

        case NI_BMI2_ZeroHighBits:
        case NI_BMI2_X64_ZeroHighBits:
        {
            assert(sig->numArgs == 2);

            GenTree* op2 = impPopStack().val;
            GenTree* op1 = impPopStack().val;
            // Instruction BZHI requires to encode op2 (3rd register) in VEX.vvvv and op1 maybe memory operand,
            // so swap op1 and op2 to unify the backend code.
            return gtNewScalarHWIntrinsicNode(retType, op2, op1, intrinsic);
        }

        case NI_BMI1_BitFieldExtract:
        case NI_BMI1_X64_BitFieldExtract:
        {
            // The 3-arg version is implemented in managed code
            if (sig->numArgs == 3)
            {
                return nullptr;
            }
            assert(sig->numArgs == 2);

            GenTree* op2 = impPopStack().val;
            GenTree* op1 = impPopStack().val;
            // Instruction BEXTR requires to encode op2 (3rd register) in VEX.vvvv and op1 maybe memory operand,
            // so swap op1 and op2 to unify the backend code.
            return gtNewScalarHWIntrinsicNode(retType, op2, op1, intrinsic);
        }

        default:
        {
            return nullptr;
        }
    }

    return retNode;
}
#endif // FEATURE_HW_INTRINSICS
