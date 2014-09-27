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

# Memo
---

디폴트 반지름: 1000km
디폴트 질량: 2 * 10^23kg = 2 * 10^5 Pt

TODO: 광원


# 약자 설명
---
I, J : 충격량
m : 질량
v : 속도
k : 관성모멘트
r : 반지름
&alpha; : 각충격량
w : 각속도

# 단위계
---

Mm - Pt - day 단위계

1 [*Pt*] = 10^15 [*t*] = 10^18 [*kg*]
1 [*Mm*] = 10^3 [*km*] = 10^6 [*m*]
1 [*day*] = 24 [*hour*] = 1440 [*minute*] = 86400 [*s*]

- 힘의 단위 *f* = *Pt \* Mm \* day^(-2)*

**만유인력 상수 G** = 6.673 \* 10^(-11) \* [*kg^(-1) \* m^3 \* s^(-2)*]
= 6.673 * 10^(-11) \* 10^18 \* [*Pt^(-1)*] \* 10^(-18) \* [*Mm^3*] \* [*s^(-2)*]
= 6.673 * 10^(-11) \* 10^18 \* 10^(-18) \* [*Pt^(-1) \* Mm^3 \* s^(-2)*]
= 6.673 * 10^(-11) \* [*Pt^(-1) \* Mm^3 \* s^(-2)*]
= 6.673 * 10^(-11) \* [*Pt^(-1) \* Mm^3*] \* (86400)^2 \* [*day^(-2)*]
= 49813678080 \* 10^(-11) \* [*Pt^(-1) \* Mm^3 \* day^(-2)*]
= **0.49813678080 \* [*f \* Pt^(-2) \* Mm^2*]**

# 충격량 (병진 운동)
---

![](https://lh6.googleusercontent.com/-6SsQg6-F33U/VCYrR2gMVnI/AAAAAAAAAA8/CdKLQkqLGSA/s0/Impulse%252528translation%252529.png "Impulse&#40;translation&#41;.png")

(단, $\vec{v}$의 $\overline{AB}$방향 선분이 $A$를 향하고 있어야 $\vec{v}$와 $\vec{J}$가 평행합니다. 그렇지 않을 경우, $\vec{v}$의 수직 방향 선분만이 충돌에 관여해 각운동량에만 변화가 일어나게 됩니다.)

역학적 에너지 보존 법칙에 따르면,

$$
\frac{1}{2}{m_B}{\left|\vec{v}\right|^2} = \frac{\left|\vec{I_A}\right|^2}{2 m_A} + \frac{\left|\vec{\alpha_A}\right|^2}{2 k_A} + \frac{\left|{m_B}\vec{v} + \vec{I_B}\right|^2}{2 m_B} + \frac{\left|\vec{\alpha_B}\right|^2}{2 k_B}
$$

정리하면,

$$
{m_B}{\left|\vec{v}\right|^2} = \frac{\left|\vec{I_A}\right|^2}{m_A} + \frac{\left|\vec{\alpha_A}\right|^2}{k_A} + \left\{ {m_B}\left|v\right|^2 + 2\left(\vec{v} \cdot \vec{I_B}\right) + \frac{\left|\vec{I_B}\right|^2}{m_B} \right\} + \frac{\left|\vec{\alpha_B}\right|^2}{k_B}
$$

$\vec{e} := \frac{\vec{BA}}{\left|\vec{BA}\right|}$ 이라고 하면

$$
{m_B}{\left|\vec{v}\right|^2} =  \frac{\left(\vec{e} \cdot \vec{J}\right)^2}{m_A} + \frac{\left|\vec{r_A} \times \vec{J}\right|^2}{k_A} +  \left\{ {m_B}\left|v\right|^2 + 2\left(\vec{v} \cdot \vec{I_B}\right) + \frac{\left| \left(-\vec{e}\right) \cdot \left(-\vec{J}\right) \right|^2}{m_B} \right\} + \frac{\left|\vec{r_B} \times \left(-\vec{J}\right) \right|^2}{k_B}
$$

정리하면,

$$
0 =  \frac{\left|\vec{J}\right|^2 \cos^2 \theta}{m_A} + \frac{{r_A}^2 \left|\vec{J}\right|^2 \sin^2 \theta}{k_A} + 2\left|\vec{v}\right| \left|\vec{I_B}\right| \cos \left(180^\circ - \theta \right) + \frac{\left|\vec{J}\right|^2 \cos^2 \theta}{m_B} + \frac{{r_B}^2 \left|\vec{J}\right|^2 \sin^2 \left(180^\circ - \theta \right)}{k_B} \\
0 =  \frac{\left|\vec{J}\right|^2 \cos^2 \theta}{m_A} + \frac{{r_A}^2 \left|\vec{J}\right|^2 \sin^2 \theta}{k_A} - 2\left|\vec{v}\right| \left|\left(-\vec{J} \cos \theta \right)\right| \cos \theta + \frac{\left|\vec{J}\right|^2 \cos^2 \theta}{m_B} + \frac{{r_B}^2 \left|\vec{J}\right|^2 \sin^2 \theta}{k_B} \\
0 =  \left\{ \frac{\cos^2 \theta}{m_A} + \frac{{r_A}^2 \sin^2 \theta}{k_A} + \frac{\cos^2 \theta}{m_B} + \frac{{r_B}^2 \sin^2 \theta}{k_B} \right\} \left|\vec{J}\right|^2 - 2\left|\vec{v}\right| \left|\vec{J}\right| \cos^2 \theta
$$

충돌이 발생했을 경우, $\vec{J}$은 $\vec{0}$이 될 수 없습니다. 따라서,

$$
|\vec{J}| = \frac{2|\vec{v}|\cos^2 \theta} {(\frac{1}{{m_a}^2} + \frac{1}{{m_b}^2})\cos^2 \theta + (\frac{{r_a}^2}{{m_a}^2} + \frac{{r_b}^2}{{k_b}^2})\sin^2 \theta}
$$

# 충격량 (회전 운동)
---

$$
$$

# 수직항력
---

![](https://lh3.googleusercontent.com/-NMuWvnl879I/VCYih1i98bI/AAAAAAAAAAk/djELgK3RY9s/s0/NormalForce.png "NormalForce.png")


$$
수직항력 = -\vec{F0} = - |\vec{F}|cos \theta \frac{\vec{r}}{|\vec{r}|} = - \frac{\vec{F} \cdot \vec{r}}{|\vec{r}|} \frac{\vec{r}}{|\vec{r}|}
$$
