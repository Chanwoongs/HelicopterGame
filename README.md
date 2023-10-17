# HelicopterGame
### C++ 콘솔로 제작한 횡스크롤 탄막 게임입니다.

<div aling="center">
  <img src="https://github.com/Chanwoongs/HelicopterGame/assets/26241243/fd6ebd3e-de73-4452-ac83-2df84a52104b" width="854" height="480"/>    
</div>

### [YouTube 영상](https://www.youtube.com/watch?v=tVCIMKQpA2w)

### 개요
> #### 벽에 부딪히지 않고, 지속적으로 나타나 총을 쏘는 적 탱크를 피해 오랫동안 살아 남기
> #### 컴포넌트 기반 개발 방법으로 설계된 프레임워크로 제작

### 개발 기간
> #### 2021.11 ~ 2021.12

### 기술
> #### [무한 지형 생성](https://github.com/Chanwoongs/HelicopterGame/blob/main/HelicopterGame/VerticalLineScript.h)
> 세로 선 여러개를 Vector에 담는 방식
> Pivot(중심), Space(빈 공간) 값을 생성 시 설정해 Pivot에서 Space 만큼의 빈 공간을 위 아래로 생성, 그 외는 3가지 다른 Terrain으로 채움
> #### [외부 메모리 단편화 방지 (updateMap 함수)](https://github.com/Chanwoongs/HelicopterGame/blob/main/HelicopterGame/Scene.h)
> 세로 선은 게임이 끝나지 않는 한 무한으로 생성됨
> 따라서 미리 MAX 이상의 수직선을 만들어놓고 맵 밖으로 나가면 SetHidden 후 초기화하여 초기 위치부터 다시 생성
> #### [충돌 (checkCollision 함수)](https://github.com/Chanwoongs/HelicopterGame/blob/main/HelicopterGame/Scene.h)
> 헬리콥터와 적의 Shape 배열에서 채워져 있는 Index를 저장
> 세로선은 빈 공간의 Top + 1과 Bottom - 1 의 포지션을 저장
> 저장한 값들을 바탕으로 헬리콥터 & 맵, 총알 & 맵, 총알 & 적 & 헬리콥터 충돌 체크
> ### Sprite Animation
> 헬리콥터 날개 회전, 적 탱크의 포신이 발사할 때 움직이는 애니메이션을 % 연산을 통해 Image들의 Index를 순환 시켜 구현

### 캡스톤 디자인
#### 컴포넌트 기반 개발 방법은 유니티, 언리얼 등 상용 엔진에서 널리 쓰이기 때문에 원리와 구조에 대해 더 깊게 배우고자 이 프로젝트에 대한 캡스톤 디자인을 실행함.
#### [결과 보고서](https://github.com/Chanwoongs/HelicopterGame/blob/main/HelicopterGame/CBD_HelicopterGame_Capstone_Design.pdf)
#### 프레임워크 대략 클래스 다이어그램
<div aling="center">
  <img src="https://github.com/Chanwoongs/HelicopterGame/assets/26241243/bd9d8628-864e-4580-bf4c-eade1e251ad6" width="854" height="250"/>    
</div>

#### 헬리콥터 게임 상세 클래스 다이어그램
<div aling="center">
  <img src="https://github.com/Chanwoongs/HelicopterGame/assets/26241243/c5532497-7aa5-4b14-9076-a4cd7350b5ad" width="900" height="600"/>    
</div>

#### 헬리콥터 게임 전체 시퀀스 다이어그램
<div aling="center">
  <img src="https://github.com/Chanwoongs/HelicopterGame/assets/26241243/4dd5b38b-daf7-4681-bf2a-87b56862e7be" width="900" height="800"/>    
</div>

#### 헬리콥터 게임 충돌 함수 시퀀스 다이어그램
<div aling="center">
  <img src="https://github.com/Chanwoongs/HelicopterGame/assets/26241243/fee1253c-debc-4b2d-aa8b-499689fd4166" width="900" height="800"/>    
</div>

### 프로젝트를 통해 얻은 점
> #### 소스 코드로만 구성된 게임 엔진 프레임 워크를 활용함으로 인해 엔진의 구조에 대한 이해도 상승
> #### Singleton 디자인 패턴 사용 이유와 방법
> #### UML 사용법과 클래스, 시퀀스 다이어그램 작성 방법
> #### 게임 로직 구현 실력 상승
> #### 상속, 맴버 초기화 리스트
> #### 객체의 생명 주기 관리와 재사용

### 아쉬운 점
> #### .h .cpp 분리
> 아직 클래스의 선언부 / 구현부 분리가 익숙하지 않아 이를 분리하는 연습이 필요할 것 같다.
> #### 충돌 체크
> 현재는 지형의 모든 부분을 For Loop에서 비교하는데 헬리콥터나 총알이 있는 Index의 지형을 찾고 필요한 크기만큼의 For Loop로 체크한다면 성능이 더욱 좋아질 것 같다.
> #### 너무 반복적인 Get 함수
> Get->Get->Get 반복이 많이 보인다. 초기에 변수 선언을 하면 더 깔끔해보이는 코드가 될 것 같다.
> #### Sleep 함수
> 현재 게임 루프 내에 Sleep 함수를 호출하여 속도 조절을 하는데, 대신 DeltaTime을 만들어 모든 하드웨어에서 일관된 프로그램이 되도록 하고 싶다.

### PS. 교수님의 피드백 ㅎ
<img src="https://github.com/Chanwoongs/HelicopterGame/assets/26241243/d83e8e56-35ff-48f1-8745-ecfdbb8e3831" width="854" height="250"/>
