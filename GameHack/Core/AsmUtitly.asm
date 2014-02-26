.386
option casemap:none
.model flat, stdcall

GetLMHNode PROTO
GetLDMNode PROTO

.DATA

.CODE

GetLMHNode PROC
pushfd
assume fs:nothing
xor edx, edx
mov ebx, fs:[30h]
mov ecx, [ebx + 0Ch]
lea edx, [ecx + 0Ch]
mov eax, edx
popfd
RET
GetLMHNode ENDP

GetLDMNode PROC
pushfd
assume fs:nothing
xor edx, edx
mov ebx, fs:[30h]
mov ecx, [ebx + 0Ch]
mov ecx, [ecx + 0Ch]
mov eax, ecx
popfd
RET
GetLDMNode ENDP

END