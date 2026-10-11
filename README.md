# Project: Fuse

## 一、项目简介

Project: Fuse 是一个 Unreal Engine 5.7 玩家能力框架 Demo，使用 Gameplay Ability System（GAS）、C++ 与蓝图实现能力系统。

项目参考《使命召唤：黑色行动 6》丧尸模式终结岛（Terminus）的能力设计。

强化（Perks）分为饮料（Drinks）、子弹模组（Ammo Mods）和主动技能（Field Abilities）。玩家可以通过地图中的 Pickup 获取三种饮料、两种子弹模组和三种主动技能。

本次范围为单人玩法，场景、射击和敌人 AI 提供基本验证环境。展示重点为饮料、子弹模组和主动技能的能力与加成框架。

## 二、目标功能

### 1. 强化（Perks）

#### 1. 饮料（Drinks）

饮料可同时持有。

| 名称 | 显示描述（中文 / English） | 补充说明 |
| --- | --- | --- |
| 强壮 / Fortitude（参考 Jugger-Nog） | 一级：最大生命值 +100。<br>Maximum health +100.<br>二级：最大生命值 +200。<br>Maximum health +200. | 基础最大生命值为 100，一级提升至 200，二级提升至 300。 |
| 加速行动 / Speed-Up（参考 Stamin-Up） | 移动速度 +50%。<br>Movement speed +50%. | 移动速度：600 → 900 cm/s。 |
| 快速充能 / Quickcharge | 主动技能冷却时间 −50%。<br>Field ability cooldowns −50%. | 冷却时间：5 → 2.5 秒。 |

#### 2. 子弹模组（Ammo Mods）

子弹模组可以同时持有。

| 名称 | 显示描述（中文 / English） | 补充说明 |
| --- | --- | --- |
| 点燃 / Ignite（参考 Napalm Burst） | 一级：命中时，50% 概率使敌人燃烧 3 秒（每秒 −10 HP）。<br>On hit, 50% chance to burn enemy for 3s (-10HP/s).<br>二级：命中时，使敌人燃烧 5 秒（每秒 −20 HP）。<br>On hit, burn enemy for 5s (-20HP/s). |  |
| 冰冻 / Freeze（参考 Cryo Freeze） | 命中时，50% 概率冻结敌人 2 秒。<br>On hit, 50% chance to freeze enemy for 2s. | 丧尸冻结期间无法移动和攻击。 |

#### 3. 主动技能（Field Abilities）

只有一个主动技能槽位，拾取另一种时替换，按 `X` 施放。主动技能共享冷却，基础时长为 5 秒。

| 名称 | 显示描述（中文 / English） | 补充说明 |
| --- | --- | --- |
| 隐身 / Shroud（参考 Aether Shroud） | 一级：隐身 1.5 秒。<br>Go undetectable for 1.5s.<br>二级：隐身 3 秒。<br>Go undetectable for 3s. | 玩家仍可移动和射击，丧尸会暂停追击和攻击。 |
| 治疗 / Heal（参考 Healing Aura） | 立即恢复全部生命值。<br>Instantly restore full health. |  |
| 放置地雷 / Place Mine（参考 Energy Mine） | 放置感应地雷，造成 50 点范围伤害。<br>Place a proximity mine that deals 50 area damage. | 触发半径 125 cm，伤害半径 300 cm；触发一次后销毁。 |

#### 4. Pickup

玩家通过拾取关卡中的 Pickup 获得 Perk，每个 Pickup 显示 Perk 的名称、颜色和描述。

首次拾取获得一级 Perk，再次拾取同种 Perk 时，未满级则升级。

### 2. 基础设施

基础设施以蓝图为主，C++ 实现 PerkData、添加/升级 Perk、查询 Perk 等级，以及 AttributeSet、死亡检测与移速属性监听。

#### 1. 玩家与输入

玩家支持前后左右移动、视角控制、开火和按 `X` 释放主动技能。

#### 2. 步枪

自动步枪支持连续开火，通过射线检测命中并造成伤害。

#### 3. 丧尸与行为树

丧尸头顶显示生命值条，右侧显示当前燃烧/冰冻状态标签。

丧尸通过行为树控制，持续追击玩家，进入攻击范围后进行近战攻击。

死亡表现使用 Timeline 下沉，随后销毁。

关卡提供丧尸生成区域，玩家进入触发区域后，在配置的五个生成点生成丧尸。

#### 4. HUD

屏幕中央显示准心，左下角显示玩家生命值。玩家 Perks 按分类展示名称、颜色、等级和对应描述，当前主动技能另显示剩余冷却时间。玩家状态区域显示隐身期间的 Status.Shrouded。

界面支持英语和简体中文。

#### 5. ESC 菜单

按 ESC 暂停游戏并显示菜单，再次按 ESC 或选择 Resume 恢复游戏；Restart 重新加载当前关卡，Quit Game 退出游戏。

## 三、具体实现

### 1. 强化（Perks）

#### 1. 饮料（Drinks）

饮料使用 Infinite GE 修改玩家属性。

| Gameplay Effect | Asset Tag | 实现 |
| --- | --- | --- |
| GE_Perk_Drink_Fortitude | Perk.Drink.Fortitude | 使用 CurveTable_Perks 按 GE 等级配置 myMaxHP 加成，一级增加 100，二级增加 200。 |
| GE_Perk_Drink_SpeedUp | Perk.Drink.SpeedUp | myMaxWalkSpeed 乘以 1.5，提高玩家移动速度。 |
| GE_Perk_Drink_Quickcharge | Perk.Drink.Quickcharge | 通过 Multiply Compound 将 myFieldAbilityCooldownDurationMultiplier 乘以 0.5，使主动技能冷却时间缩短 50%。 |

#### 2. 子弹模组（Ammo Mods）

子弹模组 GA 使用 Instanced Per Actor，在拾取时授予并激活，通过 Wait Gameplay Event 持续监听 Event.WeaponHit。处理命中事件后保持激活，供后续命中继续触发。

BP_RifleComponent 命中丧尸并造成伤害后，通过 Send Gameplay Event to Actor 向玩家发送 Event.WeaponHit，Payload 的 Instigator 为玩家，Target 为命中的丧尸。两个模组 GA 接收同一事件，使用玩家 ASC 向目标 ASC 分别施加燃烧或冻结 GE，步枪无需按具体模组分别判断。

| Gameplay Ability | Asset Tag | 实现 |
| --- | --- | --- |
| GA_Perk_AmmoMod_Ignite | Perk.AmmoMod.Ignite | 每次命中时将当前 Ability Level 传给 GE_Status_Burning，概率、持续时间和周期伤害使用 CurveTable_Perks 按等级配置。一级以 50% 概率使目标燃烧 3 秒，每秒减少 10 点 myHP；二级使目标燃烧 5 秒，每秒减少 20 点 myHP。燃烧期间赋予 Status.Burning Tag。 |
| GA_Perk_AmmoMod_Freeze | Perk.AmmoMod.Freeze | 以 50% 概率向目标施加持续 2 秒的 GE_Status_Frozen，赋予 Status.Frozen Tag，使目标停止移动和攻击。 |

状态 GE 通过 Chance to Apply 组件配置施加概率，每次命中时分别判断。

#### 3. 主动技能（Field Abilities）

三个主动技能 GA 均使用 Instanced Per Actor。激活后调用 Commit Ability，成功时执行各自效果，再调用 End Ability；提交失败时直接调用 End Ability。

| Gameplay Ability | Asset Tag | 实现 |
| --- | --- | --- |
| GA_Perk_FieldAbility_Shroud | Perk.FieldAbility.Shroud | 将当前 Ability Level 传给 GE_Shrouded，持续时间使用 CurveTable_Perks 按等级配置，一级为 1.5 秒，二级为 3 秒。通过 Granted Tags 赋予 Status.Shrouded，供状态 UI 查询。 |
| GA_Perk_FieldAbility_Heal | Perk.FieldAbility.Heal | 向玩家应用 GE_FullHeal，以目标 myMaxHP 的属性值 Override myHP，立即恢复全部生命值。 |
| GA_Perk_FieldAbility_PlaceMine | Perk.FieldAbility.Mine | 根据玩家位置与朝向生成 BP_Mine。 |

冷却使用 GE_Cooldown_FieldAbility。Duration Magnitude 为 5s × myFieldAbilityCooldownDurationMultiplier（1 → 0.5 - Quickcharge）。Granted Tags 配置 Cooldown.FieldAbility，供 GA 检查共享冷却；Asset Tags 配置同名 Tag，供 HUD 查询。

BP_Mine 的触发半径为 125 cm，Damage Sphere 半径为 300 cm。丧尸进入触发范围后播放音效，通过 BPFL_Fuse.DealDamage 对伤害范围内的丧尸各造成 50 点伤害，随后销毁地雷。

#### 4. 属性集（AttributeSet）

Perk 通过 GAS 的 Gameplay Effect（GE）修改属性，通过 Gameplay Ability（GA）执行技能逻辑。项目使用两个 C++ AttributeSet 定义相关属性：

| AttributeSet | 属性 | 用途 |
| --- | --- | --- |
| UHPAttributeSet | myHP、myMaxHP | 玩家与丧尸的当前生命值和最大生命值，供伤害、治疗和 Fortitude 加成使用。 |
| UPlayerAttributeSet | myMaxWalkSpeed、myFieldAbilityCooldownDurationMultiplier | 玩家最大行走速度和主动技能冷却时长倍率，分别供 Speed-Up 和 Quickcharge 加成使用。 |

玩家与丧尸的 ASC 分别使用 DataTable_HPAttributeSet_Player 与 DataTable_HPAttributeSet_Zombie 初始化生命值属性。玩家还使用 DataTable_PlayerAttributeSet 将最大行走速度初始化为 600 cm/s、主动技能冷却时长倍率初始化为 1。

APlayerCharacter 在 BeginPlay 将 myMaxWalkSpeed 同步到 CharacterMovement 的 MaxWalkSpeed，并绑定 ASC 的属性变化委托，在移速属性变化时同步新值。

UHPAttributeSet 在 PostGameplayEffectExecute 中检测 myHP 归零，通过 SetmyHP 设为 0，设置内部 myIsDead 后广播 OnDead；PreGameplayEffectExecute 拒绝死亡后的 HP 修改。枪击、近战和燃烧周期伤害共用该死亡检测入口。

#### 5. Pickup

BP_Pickup 负责触发 Perk 拾取，在玩家进入重叠范围时，将 PerkData 传给 C++ APlayerCharacter.AddPerk 处理。

UPerkData 继承 UDataAsset，保存名称、颜色、各等级描述，以及 GE 和 GA 的 Class 引用。饮料使用 GE，子弹模组和主动技能使用 GA。

myDescriptions 按等级保存描述，GetMaxLevel 返回数组长度减 1。项目内部等级从 0 开始，对应界面中的一级；当前等级保存在 ASC 中。

##### 强化（Perks）授予与升级：APlayerCharacter.AddPerk

APlayerCharacter.AddPerk 根据 PerkData 授予或升级 Perk，并维护 myPerks，由 AddPerkEffect 和 AddPerkAbility 分别处理 GE 和 GA。

- GE 分支：按 Class 查询已有 GE；未持有时应用 GE，已持有且未满级时提升等级。
- GA 分支：按 Class 查询 AbilitySpec；未持有时授予对应 GA，子弹模组授予后直接启动，主动技能替换当前技能并保存 Handle；已持有且未满级时提升 AbilitySpec.Level。

Perks 更新或升级后，通过 BlueprintImplementableEvent OnPerksChanged 通知 BP_PlayerCharacter 刷新 HUD 并播放拾取音效。

##### 强化（Perks）等级查询

APlayerCharacter.GetPerkLevel 按 Class 从 ASC 查询 GE / GA 的当前等级，供 UI 统一获取 Perk 等级；未持有时返回 −1。

##### 强化（Perks）分类查询

BP_PlayerCharacter.GetPerksByTag 为 HUD 提供按分类查询 myPerks 的接口。它遍历 myPerks，按 GE / GA 的 Asset Tags 匹配传入 Tag，返回对应的 PerkData。使用非精确匹配，Perk.Drink 等父 Tag 可查询整个分类。

### 2. 基础设施

#### 1. 玩家与输入

BP_PlayerCharacter 继承 C++ APlayerCharacter，使用 Enhanced Input 处理 Move、Look、Fire 和 FieldAbility；移动与视角逻辑在角色蓝图中，开火状态交给 BP_RifleComponent。按 `X` 时使用保存的 myFieldAbilitySpecHandle 调用 ASC.TryActivateAbility。

#### 2. 步枪

BP_RifleComponent 包含连续开火的状态与时间间隔判断、射线命中检测，命中后调用 BPFL_Fuse.DealDamage，并向玩家发送 Event.WeaponHit，供子弹模组 GA 响应。丧尸近战 Task 复用同一伤害函数。

#### 3. 蓝图函数库

项目通过蓝图函数库集中提供生命值、伤害和能力查询接口，供角色、技能与 UI 复用。

| 函数库 | 实现方式 | 接口 | 职责 |
| --- | --- | --- | --- |
| BPFL_Fuse | 蓝图 | GetHPData | 从目标 HPAttributeSet 读取当前生命值与最大生命值，供玩家和丧尸的生命值 UI 使用。 |
| BPFL_Fuse | 蓝图 | DealDamage | 通过 SetByCaller（Data.Damage）应用 GE_HPDamage，供步枪、地雷和丧尸近战复用。 |
| UFuseBlueprintFunctionLibrary | C++ | HasAbility | 通过 ASC.FindAbilitySpecFromClass 检查是否已拥有指定技能。 |
| UFuseBlueprintFunctionLibrary | C++ | GetAssetTags | 读取 GA Class 默认对象的 Asset Tags，供 Perk 分类查询使用。 |

#### 4. 丧尸与行为树

##### 行为树

BP_ZombieController 在 BeginPlay 中将玩家目标写入 Blackboard_Zombie。

行为树通过 Simple Parallel 同时处理追击与攻击判断：玩家可见时持续追击，进入攻击范围后按 1.5 秒间隔进行近战攻击；玩家不可见时进入等待分支。节点组织如下：

```text
Selector + BTService_CheckIfCanSeePlayer
├─ Simple Parallel（CanSeePlayer 条件，Aborts Both）
│  ├─ BTTask_ChaseTarget
│  └─ Selector + BTService_CheckIfTargetIsInAttackRange
│     ├─ BTTask_MeleeAttackOnce（范围条件 + Cooldown 1.5秒）
│     └─ Wait 0.1秒
└─ Wait 0.1秒
```

BTTask_ChaseTarget 使用 Move To Location or Actor 追击；Receive Abort AI 中调用 Stop Movement，再 Finish Abort，清理正在进行的移动。

##### 隐身（Shroud）、冻结（Freeze）与死亡响应

BTService_CheckIfCanSeePlayer 检查玩家是否拥有 GE_Shrouded，更新 CanSeePlayer；该条件使用 Observer Aborts Both，在隐身开始时中止追击和攻击，结束时恢复。

BP_ZombieCharacter 通过监听 Status.Frozen 响应冻结效果：冻结时通过 Stop Logic 停止行为树，并调用 AIController.Stop Movement 停止移动；解冻时通过 Start Logic 恢复行为树。

BP_ZombieCharacter 绑定 HPAttributeSet.OnDead：停止行为树，使用 Timeline 下沉，随后 Destroy Actor。

#### 5. HUD

UI_HUD 显示准心、玩家生命值、玩家 Perks 和主动技能冷却。玩家与丧尸的生命值展示共用 UI_HealthBar，通过 BPFL_Fuse.GetHPData 获取数据。

##### 强化（Perks）列表显示与更新

HUD 按 Drinks、Ammo Mods 和 Field Abilities 分类展示玩家 Perks。

Perks 更新或升级后，OnPerksChanged 通知 UI_HUD.RefreshPerks，刷新分类列表和当前主动技能信息。UI_PerkSection.RefreshPerks 按分类 Tag 查询 Perk 及等级，重建列表。

UI_PerkName 根据 PerkData 和等级显示名称、颜色、等级及对应描述，HUD 和场景 Pickup 共用该组件。

##### 主动技能（Field Abilities）冷却显示

HUD 每帧按 Cooldown.FieldAbility 查询冷却 GE 的剩余时间，更新主动技能倒计时。

##### 状态（Status Effects）显示

UI_StatusSection 显示 Owner 的状态标签，玩家 HUD 和丧尸的 WidgetComponent 共用该组件。每帧从 Owner 的 ASC 查询 Status 标签，仅在标签变化时刷新列表。

UI_StatusText 直接显示状态标签名：Status.Burning、Status.Frozen 和 Status.Shrouded。
