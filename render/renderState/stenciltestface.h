#pragma once

namespace renderer
{
    enum StencilOperation
    {
        StencilOperationZero,
        StencilOperationInvert,
        StencilOperationKeep,
        StencilOperationReplace,
        StencilOperationIncrement,
        StencilOperationDecrement,
        StencilOperationIncrementWrap,
        StencilOperationDecrementWrap
    };


    enum StencilTestFunction
    {
        StencilTestFunctionNever,
        StencilTestFunctionLess,
        StencilTestFunctionEqual,
        StencilTestFunctionLessThanOrEqual,
        StencilTestFunctionGreater,
        StencilTestFunctionNotEqual,
        StencilTestFunctionGreaterThanOrEqual,
        StencilTestFunctionAlways,
    };

    class StencilTestFace
    {
        public:
        //########################################
        //##### Constructors and Conversions #####
        //########################################
           inline StencilTestFace() :
               StencilFailOperation(StencilOperation::StencilOperationKeep),
               DepthFailStencilPassOperation(StencilOperation::StencilOperationKeep),
               DepthPassStencilPassOperation(StencilOperation::StencilOperationKeep),
               Function(StencilTestFunction::StencilTestFunctionAlways),
               ReferenceValue(0),
               Mask(0)
           {}

        //########################################
        //#####         Accessors            #####
        //########################################
           inline StencilOperation getStencilFailOperation() const {return StencilFailOperation;}
           inline StencilOperation getDepthFailStencilPassOperation() const {return DepthFailStencilPassOperation;}
           inline StencilOperation getDepthPassStencilPassOperation() const {return DepthPassStencilPassOperation;}

           inline StencilTestFunction getStencilFunction() const {return Function;}
           inline int getReferenceValue() const {return ReferenceValue;}
           inline int getMask() const {return Mask;}

        //########################################
        //#####           Methods            #####
        //########################################
           inline void setStencilFailOperation(StencilOperation failOp) {StencilFailOperation = failOp;}
           inline void setDepthFailStencilPassOperation(StencilOperation oper) {DepthFailStencilPassOperation = oper;}
           inline void setDepthPassStencilPassOperation(StencilOperation oper) {DepthPassStencilPassOperation = oper;}

           inline void setStencilFunction(StencilTestFunction func) {Function = func;}
           inline void setRefenceValue(int newRef){ReferenceValue = newRef;}
           inline void setMask(int newMask){Mask = newMask;}

        private:
        //########################################
        //#####           Fields             #####
        //########################################
           StencilOperation StencilFailOperation;
           StencilOperation DepthFailStencilPassOperation;
           StencilOperation DepthPassStencilPassOperation;

           StencilTestFunction Function;
           int ReferenceValue;
           int Mask;
    };
}
