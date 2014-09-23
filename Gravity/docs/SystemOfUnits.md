<!--
 Copyright (c) 2014, 임경현
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
 
 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-->

디폴트 반지름: 1000km
디폴트 질량: 2 * 10^23kg = 2 * 10^5 Pt

---

Mm - Pt - day 단위계

1Pt = 10^15t = 10^18kg
1Mm = 1000km = 1000000m = 10^6
1day = 24 hour = 1440 minute = 86400 s

- 힘의 단위 f = Pt * Mm * day^(-2)

1f = 1Pt * 1Mm/day^2

만유인력 상수 G = 6.673 * 10^(-11) * kg^(-1) * m^3 * s^(-2)
= 6.673 * 10^(-11) * 10^18 * Pt^(-1) * 10^(-18) * Mm^3 * s^(-2)
= 6.673 * 10^(-11) * 10^18 * 10^(-18) * Pt^(-1) * Mm^3 * s^(-2)
= 6.673 * 10^(-11) * Pt^(-1) * Mm^3 * s^(-2)
= 6.673 * 10^(-11) * Pt^(-1) * Mm^3 * (86400)^2 * day^(-2)
= 49813678080 * 10^(-11) * Pt^(-1) * Mm^3 * day^(-2)
= 0.49813678080 * f * Pt^(-2) * Mm^2


TODO: 광원