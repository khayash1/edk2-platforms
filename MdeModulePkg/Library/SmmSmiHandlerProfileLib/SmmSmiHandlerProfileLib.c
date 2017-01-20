/** @file
  SMM driver instance of SmiHandlerProfile Library.

  Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiSmm.h>
#include <Library/SmiHandlerProfileLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Guid/SmiHandlerProfile.h>

SMI_HANDLER_PROFILE_PROTOCOL  *mSmiHandlerProfile;

/**
  This function is called by SmmChildDispatcher module to report
  a new SMI handler is registered, to SmmCore.

  @param HandlerGuid     The GUID to identify the type of the handler.
                         For the SmmChildDispatch protocol, the HandlerGuid
                         must be the GUID of SmmChildDispatch protocol.
  @param Handler         The SMI handler.
  @param CallerAddress   The address of the module who registers the SMI handler.
  @param Context         The context of the SMI handler.
                         For the SmmChildDispatch protocol, the Context
                         must match the one defined for SmmChildDispatch protocol.
  @param ContextSize     The size of the context in bytes.
                         For the SmmChildDispatch protocol, the Context
                         must match the one defined for SmmChildDispatch protocol.

  @retval EFI_SUCCESS           The information is recorded.
  @retval EFI_UNSUPPORTED       The feature is unsupported.
  @retval EFI_OUT_OF_RESOURCES  There is no enough resource to record the information.
**/
EFI_STATUS
EFIAPI
SmiHandlerProfileRegisterHandler (
  IN EFI_GUID                       *HandlerGuid,
  IN EFI_SMM_HANDLER_ENTRY_POINT2   Handler,
  IN PHYSICAL_ADDRESS               CallerAddress,
  IN VOID                           *Context, OPTIONAL
  IN UINTN                          ContextSize OPTIONAL
  )
{
  if (mSmiHandlerProfile != NULL) {
    return mSmiHandlerProfile->RegisterHandler (mSmiHandlerProfile, HandlerGuid, Handler, CallerAddress, Context, ContextSize);
  }
  return EFI_UNSUPPORTED;
}

/**
  This function is called by SmmChildDispatcher module to report
  an existing SMI handler is unregistered, to SmmCore.

  @param HandlerGuid     The GUID to identify the type of the handler.
                         For the SmmChildDispatch protocol, the HandlerGuid
                         must be the GUID of SmmChildDispatch protocol.
  @param Handler         The SMI handler.

  @retval EFI_SUCCESS           The original record is removed.
  @retval EFI_UNSUPPORTED       The feature is unsupported.
  @retval EFI_NOT_FOUND         There is no record for the HandlerGuid and handler.
**/
EFI_STATUS
EFIAPI
SmiHandlerProfileUnregisterHandler (
  IN EFI_GUID                       *HandlerGuid,
  IN EFI_SMM_HANDLER_ENTRY_POINT2   Handler
  )
{
  if (mSmiHandlerProfile != NULL) {
    return mSmiHandlerProfile->UnregisterHandler (mSmiHandlerProfile, HandlerGuid, Handler);
  }
  return EFI_UNSUPPORTED;
}

/**
  The constructor function for SMI handler profile.

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
SmmSmiHandlerProfileLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  gSmst->SmmLocateProtocol (
           &gSmiHandlerProfileGuid,
           NULL,
           &mSmiHandlerProfile
           );
  return EFI_SUCCESS;
}
