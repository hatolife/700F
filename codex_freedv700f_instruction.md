# Codex向け指示書: FreeDV 700F相当モード探索基盤 v6

## 0. この文書の目的

この文書は, FreeDV 700Eを参考にした仮想モード `700F` の探索, 実装, 評価, 統合をCodex群に段階的に実行させるための作業指示書である.

目的は, 占有帯域幅1.9 kHz前後, 音声帯域3 kHz前後を中心に, HFのノイズ, フェージング, 周波数ずれ, 選択性フェージング, インパルスノイズを可変にしたシミュレーションを行い, 平均的なHF QSO条件でSSBより聞き取りやすい可能性のある設計点を探索することにある.

このプロジェクトは新規実装であり, 中核はC++20で実装する. Pythonは探索制御, 解析, 可視化, AI学習のために使用する. AI推論は将来の差し替えを考慮し, ONNX Runtime経由を標準とする. 各モジュールは境界プロトコルを明確化し, 将来同機能の高品質, 高性能なモジュールと差し替えられる構造にする.

この文書を読むCodexは, まずリポジトリ全体の設計とモジュール境界を作り, その後で別Codexに個別モジュール実装を分担させること.

本v3では, 複数Codexを同時運用するために, ルートディレクトリへマネージャー用 `AGENTS.md` を置き, 各モジュールサブディレクトリへ担当Codex用 `AGENTS.md` を置く方式を正式な運用モデルとした. また, 700F候補の評価対象として, SSB baselineだけでなく, FreeDV 700Dおよび700E相当のエミュレーション/参照実装比較を必須にした. さらに, 簡単な実装作業は高速なCoding向けモデルへ, 問題発生時はgpt-5.5のreasoning effortを段階的に上げるmodel routing/escalation policyを明記した.

本v4では, repository内の文書, issue, reportの運用を正式化する. 全体仕様は `./docs/`, module仕様は `./modules/NN_<name>/docs/`, 全体issueは `./issues/`, module issueは `./modules/NN_<name>/issues/`, 全体reportは `./reports/`, module reportは `./modules/NN_<name>/reports/` に保存する. すべての実装作業は, 先にMarkdown issue ticketを作成または更新してから開始する. 低頻度で, ある程度のmilestoneを達成した時点では, 監査, 不具合, セキュリティ, 再現性, 品質に関するreportを時刻付きファイル名で保存する.

本v5では, project全体と各moduleのversioning, milestone plan, TDD優先方針, GitHub CI運用を正式化する. 初期versionはprojectも全moduleも `v0.0.1` とする. `vX.Y.Z` は, `Z` を細かい変更, `Y` をそれなりに大きな機能追加および互換性cohort, `X` を破壊的変更を含むmajorとして扱う. 当面は `v0.Y.Z` 系で進め, `v1.0.0` は想定しない. module間互換は原則として `Y` が共通である場合だけ期待し, `v0.Y.Z` 全体に対する広い互換性保証は行わない. 実装はできるだけTDDで進め, GitHub CIでbuild, test, governance check, artifact生成を自動化する.

本v6では, Git運用を正式化する. 各Codexは作業ごとにissue-firstでbranchを作成し, 原則として `develop` から分岐して `develop` へPR/mergeする. `master` は日常作業では使わず, milestone snapshotまたは将来の安定release用途に限定する. 並列実装は内部moduleのgit submodule化ではなく, monorepo + module subdirectory + git worktree + branch ownershipで進める. 一方, Codec2/FreeDVのような外部上流依存は, pinningと再現性のためにGit submodule採用を推奨する. Submoduleを使う場合はManager Codexだけがpointer更新を行い, CIでrecursive checkoutとsubmodule整合性を確認する.

---

## 1. 最終ゴール

### 1.1 技術的ゴール

- C++20製の再現可能な通信シミュレーション基盤を作る.
- Codec2/FreeDV 700D相当および700E相当の基準評価を再現できるようにする.
- SSB相当のアナログ基準系を用意し, 同じチャンネル条件で比較できるようにする.
- SSB, 700D, 700E, 700F候補を同一の音声入力, 同一のchannel seed, 同一のmetric pipelineで比較できるようにする.
- `700F` 候補を複数定義し, パラメータ探索を自動化する.
- フェージング, AWGN, 周波数オフセット, サンプルクロック誤差, マルチパス, 選択性フェージング, インパルスノイズ, 隣接混信を可変化する.
- BER, FER, 同期喪失率, 再同期時間, 音声途切れ率, 遅延, 帯域幅, 音声明瞭度, ASR WER, 主観評価用出力を統一フォーマットで保存する.
- 700F候補のスコアリングとPareto frontier分析を行う.
- 将来のAIポストプロセッサ, AI音声強調, 新codec, 新FEC, 新modemをプラグインとして差し替え可能にする.
- 作業難度に応じて, Spark実装担当, gpt-5.5-low修正担当, gpt-5.5-mediumデバッグ担当, gpt-5.5-xhigh設計/難問担当へ段階的にフォールバックできる運用を作る.
- 全体仕様, module仕様, issue, reportを所定ディレクトリに保存し, issue-firstで作業する運用を作る.
- project全体versionとmodule別versionを `v0.0.1` から管理する.
- milestoneをversionと対応付け, milestone到達時にissue/report/CI結果を残す.
- 可能な限りTDDで進め, 失敗するtestを先に作ってから実装する.
- GitHub Actions CIでbuild, unit test, integration smoke test, governance checkを実行する.
- 各Codexはissueごとにbranch/worktreeを作成し, 直接 `develop` や `master` へcommitしない.
- Git Flow風の運用を採用し, `develop` を統合branch, `master` を通常未使用の保護branchにする.
- 内部16moduleは当面monorepo配下のsubdirectoryとして維持し, 外部上流依存だけGit submoduleを検討/採用する.

### 1.2 700Fの設計目標

700Fは, 700EのHFフェージング耐性を参考にしつつ, 1.9 kHz幅まで広げ, 音声帯域を3 kHz前後に絞ることで, SSB代替を狙う実用明瞭度モードとして探索する.

初期目標は次とする.

| 項目 | 目標 |
|---|---:|
| RF占有帯域幅 | 1.8 kHzから1.95 kHz |
| 音声帯域 | 300 Hzから3300 Hz前後 |
| 内部音声サンプルレート | 8 kHz基準, 必要に応じて16 kHzも許可 |
| 変調方式 | 700E系 coherent OFDM/QPSK を第一候補 |
| FEC | UEP対応LDPCまたは同等FEC |
| フレーム長 | 80 ms, 100 ms, 120 msを探索 |
| Cyclic Prefix | 4 ms, 6 ms, 8 msを探索 |
| 遅延目標 | 0.5 sから1.5 s程度までを許容 |
| 最小SNR目標 | 0 dBから3 dBで実用的 |
| フェージング目標 | 700E同等以上を狙う |
| 音声品質目標 | 700Eより明確に良い. 平均的SSBより聞き取りやすい領域を探す |

注意: 700Fの目標は, 全条件でSSBを完全に上回ることではない. 目標は, 平均的HF QSO条件でSSBより狭帯域, 低ノイズ, 高明瞭に感じられる領域を探すことである.

---

## 2. 外部前提と検証義務

Codexは作業開始時点で, 以下を最新のリポジトリまたは公式ドキュメントで確認すること.

- Codec2/FreeDV本体の現在の構成.
- FreeDV APIの現在のモード定義とサンプルプログラム.
- 700Eの現在の定数, キャリア数, OFDM設定, FEC設定, サンプルレート, フレーム長.
- Codec2のライセンスとリンク形態の制約.
- ONNX Runtime C++ APIの導入方法.

この指示書内の数値は初期設計仮説であり, 公式リポジトリの現在値と矛盾する場合は, ソースコードを優先する. ただし設計理由と差分を必ず `docs/verification_notes.md` に記録すること.

---

## 3. 全体アーキテクチャ

### 3.1 推奨リポジトリ構成

```text
freedv700f-lab/
  AGENTS.md                 # Manager Codex用. 全体統制, 統合, review, merge policyを記述.
  VERSION                   # project version. 初期値 v0.0.1.
  versions.toml             # project/module/protocol/ABI version manifest.
  CHANGELOG.md              # project全体の変更履歴.
  .gitmodules               # 外部上流依存をsubmoduleにする場合のみ使用.
  .github/
    workflows/
      ci.yml                # PR/push用: configure, build, unit/integration smoke test.
      governance.yml        # issue/docs/report/version整合性check.
      nightly-sweep.yml     # schedule/manual用: 重いsweepとartifact upload.
      security.yml          # dependency/static/security check.
  .codex/
    agents/
      spark-implementer.toml
      low-fixer.toml
      medium-debugger.toml
      xhigh-architect.toml
      reviewer.toml
  CMakeLists.txt
  cmake/
  docs/
    README.md
    architecture.md
    versioning.md
    milestones.md
    tdd_policy.md
    ci.md
    git_workflow.md
    submodule_policy.md
    codec2_verification_notes.md
    protocol.md
    module_contracts.md
    simulation_method.md
    scoring_method.md
    codex_task_log.md
    coordination/
      manager_log.md
      model_routing.toml
      escalation_log.md
      interface_change_requests.md
      merge_queue.md
      branches.md
      locks.md
    tasks/
      module_01_repository_build_ci.md
      module_02_core_data_model.md
      ...
      module_16_golden_benchmark_integration_qa.md
  issues/
    README.md                         # 全体issue index. Manager Codexが管理.
    ISSUE-0001-bootstrap-repository.md
    ISSUE-0002-verify-codec2-baselines.md
  external/
    codec2/                 # 推奨: Codec2/FreeDV upstreamをpinするGit submodule.
  patches/
    codec2/                 # submoduleへ直接commitしないためのpatch置き場.
  modules/
    01_repository_build_ci/
      AGENTS.md
      VERSION
      CHANGELOG.md
      CMakeLists.txt
      README.md
      include/
      src/
      tests/
      docs/
        README.md
        module_spec.md
        versioning.md
        testing.md
      issues/
        README.md
      reports/
        README.md
    02_core_data_model_protocol/
      AGENTS.md
      VERSION
      CHANGELOG.md
      CMakeLists.txt
      README.md
      include/
      src/
      tests/
      docs/
        README.md
        module_spec.md
        versioning.md
        testing.md
      issues/
        README.md
      reports/
        README.md
    03_plugin_abi_registry/
      AGENTS.md
      VERSION
      CHANGELOG.md
      CMakeLists.txt
      README.md
      include/
      src/
      tests/
      docs/
        README.md
        module_spec.md
        versioning.md
        testing.md
      issues/
        README.md
      reports/
        README.md
    04_audio_io_dataset/
      AGENTS.md
      VERSION
      CHANGELOG.md
      CMakeLists.txt
      README.md
      include/
      src/
      tests/
      docs/
        README.md
        module_spec.md
        versioning.md
        testing.md
      issues/
        README.md
      reports/
        README.md
    05_codec_adapter/
      AGENTS.md
      VERSION
      CHANGELOG.md
      CMakeLists.txt
      README.md
      include/
      src/
      tests/
      docs/
        README.md
        module_spec.md
        versioning.md
        testing.md
      issues/
        README.md
      reports/
        README.md
    06_fec_uep_engine/
      AGENTS.md
      VERSION
      CHANGELOG.md
      CMakeLists.txt
      README.md
      include/
      src/
      tests/
      docs/
        README.md
        module_spec.md
        versioning.md
        testing.md
      issues/
        README.md
      reports/
        README.md
    07_ofdm_modem_waveform/
      AGENTS.md
      VERSION
      CHANGELOG.md
      CMakeLists.txt
      README.md
      include/
      src/
      tests/
      docs/
        README.md
        module_spec.md
        versioning.md
        testing.md
      issues/
        README.md
      reports/
        README.md
    08_hf_channel_simulator/
      AGENTS.md
      VERSION
      CHANGELOG.md
      CMakeLists.txt
      README.md
      include/
      src/
      tests/
      docs/
        README.md
        module_spec.md
        versioning.md
        testing.md
      issues/
        README.md
      reports/
        README.md
    09_sync_impairment_estimator/
      AGENTS.md
      VERSION
      CHANGELOG.md
      CMakeLists.txt
      README.md
      include/
      src/
      tests/
      docs/
        README.md
        module_spec.md
        versioning.md
        testing.md
      issues/
        README.md
      reports/
        README.md
    10_reference_baselines_ssb_700d_700e/
      AGENTS.md
      VERSION
      CHANGELOG.md
      CMakeLists.txt
      README.md
      include/
      src/
      tests/
      docs/
        README.md
        module_spec.md
        versioning.md
        testing.md
      issues/
        README.md
      reports/
        README.md
    11_pipeline_runner/
      AGENTS.md
      VERSION
      CHANGELOG.md
      CMakeLists.txt
      README.md
      include/
      src/
      tests/
      docs/
        README.md
        module_spec.md
        versioning.md
        testing.md
      issues/
        README.md
      reports/
        README.md
    12_metrics_scoring/
      AGENTS.md
      VERSION
      CHANGELOG.md
      CMakeLists.txt
      README.md
      include/
      src/
      tests/
      docs/
        README.md
        module_spec.md
        versioning.md
        testing.md
      issues/
        README.md
      reports/
        README.md
    13_sweep_optimization/
      AGENTS.md
      VERSION
      CHANGELOG.md
      CMakeLists.txt
      README.md
      include/
      src/
      tests/
      docs/
        README.md
        module_spec.md
        versioning.md
        testing.md
      issues/
        README.md
      reports/
        README.md
    14_ai_integration/
      AGENTS.md
      VERSION
      CHANGELOG.md
      CMakeLists.txt
      README.md
      include/
      src/
      tests/
      docs/
        README.md
        module_spec.md
        versioning.md
        testing.md
      issues/
        README.md
      reports/
        README.md
    15_reporting_visualization/
      AGENTS.md
      VERSION
      CHANGELOG.md
      CMakeLists.txt
      README.md
      include/
      src/
      tests/
      docs/
        README.md
        module_spec.md
        versioning.md
        testing.md
      issues/
        README.md
      reports/
        README.md
    16_golden_benchmark_integration_qa/
      AGENTS.md
      VERSION
      CHANGELOG.md
      CMakeLists.txt
      README.md
      include/
      src/
      tests/
      docs/
        README.md
        module_spec.md
        versioning.md
        testing.md
      issues/
        README.md
      reports/
        README.md
  include/
    f700f/                  # 統合後に公開される安定header. 原則Manager Codexが管理.
  src/
    apps/                   # CLI entrypoints. 原則Manager Codexが管理.
  python/
    f700f_lab/
      __init__.py
      sweep.py
      analyze.py
      plot.py
      asr_wer.py
      train_ai.py
      export_onnx.py
    pyproject.toml
  configs/
    modes/
      ssb_standard_3k.toml
      ssb_narrow_1p9k.toml
      700d_baseline.toml
      700e_baseline.toml
      700f_a.toml
      700f_b.toml
      700f_c.toml
    channels/
      awgn.toml
      hf_mild.toml
      hf_moderate.toml
      hf_severe.toml
      dx_selective.toml
    sweeps/
      initial_sweep.toml
      reference_baseline_sweep.toml
      pareto_sweep.toml
  datasets/
    README.md
    speech_manifest.example.toml
  tests/
    integration/
    golden/
  tools/
    run_initial_sweep.sh
    run_reference_baselines.sh
    package_report.sh
  reports/
    README.md                         # 全体report index. Manager Codexが管理.
    .gitkeep
    YYYYMMDDTHHMMSSZ_project_audit.md
    YYYYMMDDTHHMMSSZ_phase_<n>_milestone.md
```

### 3.2 採用技術

| 領域 | 採用 |
|---|---|
| Core language | C++20 |
| Build | CMake 3.25以上 |
| Package管理 | vcpkgまたはFetchContent. 片方に寄せること |
| 設定ファイル | TOML |
| 結果ログ | JSONL + CSV summary |
| 音声ファイル | WAV PCM16/PCM32F |
| IQファイル | CF32 interleaved little-endian |
| AI推論 | ONNX Runtime, optional |
| AI学習 | Python + PyTorch, optional |
| Python | 3.11以上 |
| テスト | CTest + doctestまたはCatch2 |
| フォーマット | clang-format, clang-tidy, ruff, mypy optional |

### 3.3 基本方針

- 実験の再現性を最優先する.
- 乱数は必ずseedを記録する.
- すべての実験結果はmanifest, config hash, git commit hash, dependency versionを含める.
- 直接具象クラスに依存しない. すべての主要機能はinterface + descriptor経由にする.
- C++内部interfaceとC ABI plugin interfaceの両方を用意する.
- 研究初期ではC++内部実装を優先し, 将来の差し替えのためにC ABIを固定する.
- 700Fの探索は, 700E, 2020相当, SSB相当との比較ができる形で行う.
- AIは必須経路にしない. AIなしでも全シミュレーションが動くこと.
- AIはpostprocess, estimator, decoder helper, scoring helperとして後付け可能にする.


### 3.4 複数Codex運用モデル

このプロジェクトは, 1つの巨大なCodexに全作業を持たせない. ルートで稼働するManager Codexと, 各 `modules/NN_<name>/` で稼働するModule Codexを分離する.

| 役割 | 実行場所 | 主な責務 | 書き換え可能範囲 |
|---|---|---|---|
| Manager Codex | repository root | 全体設計, interface承認, root CMake, integration, merge, task発行, 最終QA | root files, `docs/`, `include/f700f/`, `src/apps/`, `configs/`, `tests/integration/` |
| Module Codex | 各 `modules/NN_<name>/` | 担当module実装, unit test, module docs, local benchmark | 自分のmodule directory内 |
| Integration Codex optional | repository root | 複数moduleを結合して壊れたbuild/testを直す | Manager Codexが明示した範囲のみ |

Manager Codexは, 各モジュールの `AGENTS.md` を生成してから作業を分担する. Module Codexは, 自分の作業開始時に必ず次を読む.

1. repository rootの `AGENTS.md`.
2. 自分の `modules/NN_<name>/AGENTS.md`.
3. `docs/protocol.md`.
4. `docs/module_contracts.md`.
5. 自分用の `docs/tasks/module_NN_<name>.md`.

Module Codexは, 原則として他moduleの中身を直接編集しない. 変更が必要な場合は `docs/coordination/interface_change_requests.md` にRFC形式で提案し, Manager Codexが承認してから変更する.

### 3.5 AGENTS.mdの生成方針

`AGENTS.md` はCodexへの実行時指示であり, 人間向けREADMEとは分離する. ルートの `AGENTS.md` は全体統制用, 各moduleの `AGENTS.md` は担当Codex用である.

Manager CodexはPhase 1で以下を必ず生成する.

```text
AGENTS.md
modules/01_repository_build_ci/AGENTS.md
modules/02_core_data_model_protocol/AGENTS.md
...
modules/16_golden_benchmark_integration_qa/AGENTS.md
```

各 `AGENTS.md` は次を満たすこと.

- 守るべき境界を明記する.
- 触ってよいpathと触ってはいけないpathを明記する.
- moduleのpublic API変更手順を明記する.
- test, format, lint, build commandを明記する.
- 完了時に更新すべきlogを明記する.
- 未実装stubを残す場合の記録方法を明記する.

### 3.6 Git branch, worktree, submodule運用規約

このプロジェクトは複数Codexが並列に作業する. 競合と履歴汚染を避けるため, **issue-first + branch-per-task + worktree推奨** を採用する.

#### 3.6.1 Branch model

Git Flow風の運用を採用するが, 厳密な古典的Git Flowではなく, AI並列開発向けに簡略化する.

| Branch | 用途 | 直接commit | 備考 |
|---|---|---|---|
| `develop` | 日常の統合branch | 禁止. PR/mergeのみ | 全feature/fix/docs/choreのmerge先 |
| `master` | 通常未使用の保護branch | 禁止 | milestone snapshotや将来release用途だけ. 当面は使わない |
| `feature/...` | 新機能 | 作業Codexのみ可 | issue単位 |
| `fix/...` | bug fix | 作業Codexのみ可 | issue単位 |
| `chore/...` | build/CI/docs整理 | 作業Codexのみ可 | issue単位 |
| `docs/...` | docsのみ | 作業Codexのみ可 | issue単位 |
| `research/...` | 実験/調査 | 作業Codexのみ可 | merge前に通常branchへ整理 |
| `integration/...` | Managerによる複数branch統合検証 | Managerのみ可 | 一括merge前の衝突確認用 |
| `release/v0.Y.0` | milestone締め | Managerのみ可 | 必要になった場合だけ |

`master` は基本的に使わない. もしGitHub上でdefault branchが `master` になっている場合でも, Manager Codexは開発のbase branchを `develop` に設定する. `master` へのmergeやtagは, milestone reportとrelease-checkがgreenになった場合だけ許可する.

#### 3.6.2 Branch naming

branch名はissue idとmodule番号を含める.

```text
feature/m08/ISSUE-0003-add-awgn-model
fix/m12/ISSUE-0011-fix-wer-json-output
chore/m01/ISSUE-0007-add-ci-governance-check
docs/project/ISSUE-0009-write-submodule-policy
research/m07/ISSUE-0021-ofdm-carrier-spacing-probe
integration/20260627-batch-01
release/v0.3.0
```

原則:

- 1 issue = 1 branch.
- 1 branch = 1 owner Codex.
- branch名に空白を入れない.
- branch名にmodule番号または `project` を入れる.
- 作業が膨らんだ場合は新issueと新branchに分割する.

#### 3.6.3 Worktree運用

並列Codex実行では, git submoduleではなく `git worktree` で独立作業場所を作ることを推奨する.

Manager Codexの例:

```bash
git checkout develop
git pull --ff-only
git worktree add ../f700f-wt-m08-issue-0003 -b feature/m08/ISSUE-0003-add-awgn-model develop
```

Module Codexは, 渡されたworktreeで作業する.

```bash
cd ../f700f-wt-m08-issue-0003/modules/08_hf_channel_simulator
```

worktreeはrepository内ではなく, repository隣接directoryに置くことを推奨する.

```text
../f700f-wt-manager/
../f700f-wt-m01-issue-0001/
../f700f-wt-m08-issue-0003/
../f700f-wt-m12-issue-0011/
```

issueには必ず次を記録する.

```markdown
## Branch
- Base branch: develop
- Work branch: feature/m08/ISSUE-0003-add-awgn-model
- Worktree: ../f700f-wt-m08-issue-0003
- Owner Codex: module-08
```

#### 3.6.4 作業開始手順

各Codexは, 実装前に次を行う.

1. 対応issueを作成または更新する.
2. issue Statusを `in_progress` にする.
3. base branchが `develop` であることを確認する.
4. issue idを含むbranchを作成する.
5. 必要ならworktreeを作成する.
6. issueにbranch/worktree/ownerを書く.
7. TDD方針に従い, 先にtestまたはgolden expectationを書く.
8. 実装する.
9. local testを実行する.
10. PR/merge request相当のhandoffを作成する.

#### 3.6.5 Merge policy

- feature/fix/docs/chore branchは `develop` へmergeする.
- `develop` と `master` へ直接commitしてはいけない.
- defaultはsquash mergeを推奨する. commit messageにissue id, module, version bump有無を入れる.
- 複数moduleにまたがるintegration branchは, Manager Codexが作り, CIで検証してから `develop` へmergeする.
- merge前にはFast CIとgovernance checkがgreenであること.
- module public API, protocol, ABI, file formatに影響する変更は, xhigh-architectまたはManager reviewを必要とする.
- branch merge後は不要なworktreeを削除する.

worktree削除例:

```bash
git worktree remove ../f700f-wt-m08-issue-0003
```

#### 3.6.6 Lock and conflict policy

並列作業で同じfileを編集しそうな場合, Manager Codexは `docs/coordination/locks.md` にsoft lockを記録する.

```markdown
| Path | Owner | Issue | Branch | Expires UTC | Reason |
|---|---|---|---|---|---|
| include/f700f/protocol.hpp | manager | ISSUE-0010 | integration/20260627-batch-01 | 2026-06-28T00:00:00Z | protocol update |
```

soft lockは絶対禁止ではないが, 他Codexは触る前にManagerへescalateする.

#### 3.6.7 Submodule採用方針

結論:

- **内部16moduleは当面Git submoduleにしない.** Monorepo内のsubdirectoryとして実装する.
- **外部上流依存はGit submoduleを使ってよい.** 特にCodec2/FreeDVはbaseline再現性のため, `external/codec2` としてpinする運用を推奨する.
- 並列Codex作業のためにsubmoduleを使うのではなく, branch/worktreeを使う.

内部moduleをsubmoduleにしない理由:

- module境界がv0期間で頻繁に変わる.
- cross-module変更をatomicにcommitしづらくなる.
- CIとversion cohort管理が複雑になる.
- Codexがsubmodule pointer更新と実体commitを混同しやすい.
- `v0.Y.Z` のY cohort互換性をmonorepoでまとめて検査しやすい.

内部moduleを将来submodule化してよい条件:

- module public APIが安定している.
- そのmodule単独でCI, tests, release tag, changelogを持つ.
- cross-module変更が少ない.
- C ABIまたは明確なfile protocolだけで接続できる.
- Manager Codexが `docs/submodule_policy.md` に移行計画を書き, milestone issueで承認する.

#### 3.6.8 外部submodule運用

Codec2/FreeDVをsubmoduleとして使う場合:

```bash
git submodule add <codec2-upstream-or-fork-url> external/codec2
git submodule update --init --recursive
```

ただしCodexは, URLやremoteを推測で固定してはいけない. Manager Codexが `docs/codec2_verification_notes.md` に採用remote, commit hash, license, baseline検証結果を記録してから `.gitmodules` を更新する.

submodule更新手順:

1. root issueを作る. 例: `ISSUE-00XX-update-codec2-submodule.md`.
2. branchを作る. 例: `chore/project/ISSUE-00XX-update-codec2-submodule`.
3. `external/codec2` のcommitを更新する.
4. `.gitmodules` とsubmodule pointer差分を確認する.
5. `docs/codec2_verification_notes.md` に旧commit, 新commit, 変更理由, baseline影響を書く.
6. 700D/700E reference baseline smoke testを実行する.
7. CIをgreenにする.
8. Manager review後に `develop` へmergeする.

submodule内に直接patchをcommitしない. patchが必要な場合は次のどちらかにする.

- forkを作り, forkのbranch/tagをsubmoduleとしてpinする.
- `patches/codec2/*.patch` にpatchを置き, apply手順を `docs/codec2_verification_notes.md` に書く.

#### 3.6.9 CIでのsubmodule扱い

GitHub CIは, submoduleを使う場合に備えてrecursive checkoutを有効化する. YAMLの具体versionは実装時点でManager Codexが確認し, `docs/ci.md` に記録する.

CI内では最低限次を行う.

```bash
git submodule update --init --recursive
python tools/check_versions.py
python tools/check_submodules.py
```

`tools/check_submodules.py` は次を確認する.

- `.gitmodules` に記載されたpathが存在する.
- `external/codec2` が存在する場合, commit hashがrun manifestに記録される.
- submodule working treeに未commit変更がない.
- submodule update issueなしにpointerが変わっていない.

#### 3.6.10 Branch/submodule関連docs

Manager Codexは次を作る.

```text
docs/git_workflow.md
docs/submodule_policy.md
docs/coordination/branches.md
docs/coordination/locks.md
tools/check_submodules.py
```

`docs/coordination/branches.md` は現在のbranch owner表を持つ.

```markdown
| Issue | Branch | Owner | Worktree | Status | Merge target |
|---|---|---|---|---|---|
| ISSUE-0003 | feature/m08/ISSUE-0003-add-awgn-model | module-08 | ../f700f-wt-m08-issue-0003 | in_progress | develop |
```

---


### 3.7 Docs, Issues, Reports 運用規約

本プロジェクトでは, Codexが複数並行で作業するため, 仕様, issue, reportの所在を固定する. 口頭の指示やCodexの一時的な会話内容ではなく, repository内のMarkdown文書を長期的な作業記録のsource of truthとする.

#### 3.7.1 ディレクトリ責務

| Path | 管理者 | 内容 | 更新頻度 |
|---|---|---|---|
| `./docs/` | Manager Codex | 全体仕様, architecture, protocol, module contract, scoring, simulation method | 仕様変更時に必ず更新 |
| `./issues/` | Manager Codex | 全体issue, cross-module issue, integration issue, architecture issue | 作業前に必ず作成/更新 |
| `./reports/` | Manager Codex | 全体milestone report, audit report, security report, integration QA report | milestone達成時に低頻度で作成 |
| `./modules/NN_<name>/docs/` | Module Codex | module固有仕様, module API, module internal design, module testing method | module仕様変更時に必ず更新 |
| `./modules/NN_<name>/issues/` | Module Codex | module内作業ticket, bug ticket, refactor ticket, test ticket | module作業前に必ず作成/更新 |
| `./modules/NN_<name>/reports/` | Module Codex | module audit, module QA, module benchmark, module milestone report | module milestone達成時に低頻度で作成 |

`README.md` は各 `docs/`, `issues/`, `reports/` directoryのindexとして使う. Manager CodexまたはModule Codexは, 新規文書を追加した場合, 同じdirectoryの `README.md` を更新すること.

#### 3.7.2 全体仕様書とmodule仕様書

`./docs/` には, 少なくとも次の全体仕様書を置く.

| File | 内容 |
|---|---|
| `docs/README.md` | 全体仕様書index. どの文書を読めばよいかを一覧化する |
| `docs/architecture.md` | システム全体構成, module依存, data flow |
| `docs/protocol.md` | C++ interface, C ABI, file protocol, config protocol |
| `docs/module_contracts.md` | 各moduleの責務, input/output, ownership |
| `docs/simulation_method.md` | channel model, SNR定義, SSB/700D/700E/700F比較方法 |
| `docs/scoring_method.md` | metrics, score, Pareto分析, pass/fail閾値 |
| `docs/reporting.md` | report生成条件, file名, template, review手順 |
| `docs/issue_workflow.md` | issue-first運用, status, priority, escalation |
| `docs/security.md` | plugin, external dependency, file IO, AI model, generated artifactの安全方針 |
| `docs/coordination/*.md` | Codex間coordination, manager log, escalation log, interface RFC |

各moduleの `./modules/NN_<name>/docs/` には, 少なくとも次を置く.

| File | 内容 |
|---|---|
| `docs/README.md` | module仕様書index |
| `docs/module_spec.md` | module目的, 非目的, input/output, internal design |
| `docs/api_contract.md` | module公開APIとprotocol適合条件 |
| `docs/testing.md` | unit/integration/golden test方針 |
| `docs/limitations.md` | 既知制限, mock/stub, 未実装, 仮定 |

module仕様が全体protocolに影響する場合, module docsだけでなく `./docs/protocol.md` と `./docs/module_contracts.md` も更新する. Module Codexが直接全体仕様を変更できない場合は, `docs/coordination/interface_change_requests.md` にRFCを作成し, Manager Codexの承認を待つ.

#### 3.7.3 Issue-first作業ルール

すべての作業は, 先にissue ticketを作成または更新してから開始する. これは, 実装, refactor, docs更新, test追加, bug修正, report作成, config変更, CI変更のすべてに適用する.

例外は, repository初期化直後に `issues/README.md` と最初のbootstrap issueを作成する作業だけである. その場合でも, 最初のcommitに `issues/ISSUE-0001-bootstrap-repository.md` を含めること.

issueの置き場所は次で判断する.

| Issue種別 | 置き場所 | 例 |
|---|---|---|
| 全体設計, protocol, cross-module, integration, CI, release | `./issues/` | `ISSUE-0007-public-protocol-freeze.md` |
| module内実装, module bug, module test, module docs | `./modules/NN_<name>/issues/` | `ISSUE-0003-add-awgn-channel.md` |
| module間interface変更提案 | `./docs/coordination/interface_change_requests.md` と関連issue | `IFACE-RFC-0002-add-soft-symbols.md` |
| escalation | `docs/coordination/escalation_log.md` と関連issue | `ESC-20260627-001` |

issue file名は, 原則として次の形式にする.

```text
ISSUE-<4-digit-id>-<short-kebab-slug>.md
```

同一directory内でIDを単調増加させる. root issueとmodule issueのID空間は別でよい. 例:

```text
issues/ISSUE-0001-bootstrap-repository.md
modules/08_hf_channel_simulator/issues/ISSUE-0001-add-awgn-model.md
modules/10_reference_baselines_ssb_700d_700e/issues/ISSUE-0002-validate-700e-emulator.md
```

#### 3.7.4 Issue ticket template

各issueは次のtemplateに従う.

```markdown
# ISSUE-0000: <short title>

## Status
open | in_progress | blocked | review | closed | deferred

## Scope
project | module:<NN_name> | integration | docs | qa | security | research

## Priority
P0 | P1 | P2 | P3

## Owner
- Codex role: Manager | Module NN | Integration | Reviewer | Research
- Model tier: spark-implementer | low-fixer | medium-debugger | xhigh-architect

## Branch
- Base branch: develop
- Work branch:
- Worktree:
- Merge target: develop
- PR/Merge status: none | open | merged | abandoned

## Target version and milestone
- Project version:
- Module version:
- Compatibility line:
- Milestone:

## Submodule impact
none | external/codec2 pointer update | patch only | policy update

## Background
<なぜ必要か. 関連するユーザー指示, 仕様, 既知問題.>

## Task
<実装または調査する内容.>

## Affected paths
- <path>

## Dependencies
- <issue id or none>

## Acceptance criteria
- [ ] <観測可能な完了条件>
- [ ] build/test/docsが通る
- [ ] 関連README/indexを更新した

## Test plan
- <実行するcommand>

## TDD record
- Red test added:
- Red command:
- Expected failure:
- Green command:
- Refactor notes:
- TDD exception if any:

## Work log
- <YYYY-MM-DD HH:MM UTC> Created.

## Result
<完了時に結果, commit, report link, limitationsを書く.>
```

作業中に新しいTODO, bug, design debtを発見した場合, コメントだけで放置せず, 関連issueを作成する. TODOコメントを残す場合は, 必ず `TODO(ISSUE-0000): ...` のようにissue idを含める.

#### 3.7.5 Issue README運用

各 `issues/README.md` は同directory内issueのindexである. 新規issue作成時, status変更時, close時に必ず更新する.

root `issues/README.md` は次の列を持つ.

```markdown
# Project Issues

| ID | Status | Priority | Scope | Title | Owner | Branch | Target version | Milestone | Depends | Last updated |
|---|---|---|---|---|---|---|---|---|---|---|
| ISSUE-0001 | open | P0 | project | Bootstrap repository | Manager | chore/project/ISSUE-0001-bootstrap-repository | v0.0.1 | M0 | - | 2026-06-27 |
```

module `issues/README.md` も同様にするが, Scopeはmodule名を省略してよい.

Manager Codexは, 低頻度でissue indexの整合性を監査し, 存在しないissue, close済みなのにREADMEでopenのissue, README未登録のissueを修正する.

#### 3.7.6 Report保存ルール

reportは頻繁に作りすぎない. 作業ログはissueに残し, reportはmilestone, audit, QA, benchmark, security reviewなど, まとまった判断が必要な時点で作成する.

report作成トリガは次とする.

| Trigger | 保存先 | 例 |
|---|---|---|
| Phase完了 | `./reports/` | Phase 1 foundation complete |
| baseline比較完了 | `./reports/` | SSB/700D/700E baseline validation |
| 初期sweep完了 | `./reports/` | 700F-A/B/C initial sweep |
| integration前QA | `./reports/` | public protocol freeze audit |
| security/dependency audit | `./reports/` | external dependency audit |
| module milestone完了 | `./modules/NN_<name>/reports/` | channel model v1 complete |
| module bug/security audit | `./modules/NN_<name>/reports/` | module QA/security review |

report file名はUTC時刻付きで次の形式にする.

```text
YYYYMMDDTHHMMSSZ_<scope>_<kind>.md
```

例:

```text
reports/20260627T093000Z_project_phase1_foundation_audit.md
reports/20260628T210500Z_project_reference_baseline_validation.md
modules/08_hf_channel_simulator/reports/20260627T143000Z_module08_channel_model_v1_audit.md
modules/10_reference_baselines_ssb_700d_700e/reports/20260628T101500Z_module10_700e_emulator_validation.md
```

#### 3.7.7 Report template

reportは次のtemplateに従う.

```markdown
# <scope> <kind> report - <YYYY-MM-DD HH:MM:SS UTC>

## Summary
<結論を先に書く. 達成したこと, 未解決のこと, 次の判断.>

## Scope
- Target: project | module NN | integration | security | benchmark
- Related issues:
- Related reports:

## Environment
- Git commit:
- Base branch:
- Work branch:
- Project version:
- Module versions:
- Submodule commits:
- Build type:
- OS/compiler:
- Dependency versions:
- Config files:
- Random seeds:

## Checks performed
- [ ] Build
- [ ] Unit tests
- [ ] Integration tests
- [ ] Golden/regression tests
- [ ] Static analysis
- [ ] Sanitizer if applicable
- [ ] Security/dependency check if applicable
- [ ] Reproducibility check

## Results
<数値, table, log summary, artifact path.>

## Defects and risks
<不具合, セキュリティ懸念, data loss, reproducibility risk, simulation validity risk.>

## Security notes
<plugin ABI, external files, path traversal, untrusted model, generated artifact, dependency license.>

## Decision
proceed | proceed_with_risks | blocked | rollback_required

## Follow-up issues
- <issue id>
```

#### 3.7.8 Reports README運用

`reports/README.md` と各moduleの `reports/README.md` はreport indexである. 新規report作成時に必ず更新する.

```markdown
# Reports

| Timestamp UTC | Scope | Kind | Decision | Related issues | File |
|---|---|---|---|---|---|
| 2026-06-27 09:30:00 | project | phase1_foundation_audit | proceed | ISSUE-0001 | 20260627T093000Z_project_phase1_foundation_audit.md |
```

#### 3.7.9 Issue, report, escalationの関係

- issueは作業単位であり, 日常的に作る.
- reportはmilestoneや監査単位であり, 低頻度で作る.
- escalationは問題解決の引き継ぎ単位であり, 関連issueに必ずリンクする.
- report内で発見した問題は, 必ずissue化する.
- issueをcloseするとき, reportに根拠がある場合はreport linkを書く.

#### 3.7.10 Codex運用への組み込み

Manager Codexは, 下流Codexへtaskを渡す前に, 対応するroot issueまたはmodule issueを作成する. Module Codexは, 実装開始前にそのissueのStatusを `in_progress` にし, 作業完了時に `review` または `closed` に更新する.

Module Codexが自分で小さな追加taskを見つけた場合は, 自分の `modules/NN_<name>/issues/` に新規issueを作る. ただしpublic interfaceや他moduleに影響する場合は, module issueでは完結させず, Managerへescalateする.

review Codexは, pull/merge前に次を確認する.

- 変更に対応するissueがある.
- issueのAcceptance criteriaが満たされている.
- 変更したdocs/issues/reportsのREADME indexが更新されている.
- milestone相当の変更ならreportが作成されている.
- TODOにissue idが付いている.
- 全体protocolやmodule contractが実装と矛盾していない.


### 3.8 Versioning, milestones, TDD, GitHub CI 運用規約

#### 3.8.1 Versioning基本方針

project全体と各moduleは, それぞれ独立したversionを持つ. 初期versionはすべて `v0.0.1` とする.

| 対象 | version file | changelog | 管理者 |
|---|---|---|---|
| project全体 | `VERSION` | `CHANGELOG.md` | Manager Codex |
| module | `modules/NN_<name>/VERSION` | `modules/NN_<name>/CHANGELOG.md` | Module Codex, Manager承認 |
| version一覧 | `versions.toml` | n/a | Manager Codex |
| version仕様 | `docs/versioning.md` | n/a | Manager Codex |

`VERSION` fileは1行だけを持つ.

```text
v0.0.1
```

`versions.toml` は次を基準にする.

```toml
project = "v0.0.1"
protocol = "f700f-protocol-0.0"
plugin_abi = "f700f-plugin-abi-0.1"
compatibility_minor = 0

[modules]
repository_build_ci = "v0.0.1"
core_data_model_protocol = "v0.0.1"
plugin_abi_registry = "v0.0.1"
audio_io_dataset = "v0.0.1"
codec_adapter = "v0.0.1"
fec_uep_engine = "v0.0.1"
ofdm_modem_waveform = "v0.0.1"
hf_channel_simulator = "v0.0.1"
sync_impairment_estimator = "v0.0.1"
reference_baselines_ssb_700d_700e = "v0.0.1"
pipeline_runner = "v0.0.1"
metrics_scoring = "v0.0.1"
sweep_optimization = "v0.0.1"
ai_integration = "v0.0.1"
reporting_visualization = "v0.0.1"
golden_benchmark_integration_qa = "v0.0.1"
```

#### 3.8.2 `vX.Y.Z` の意味

| 要素 | 意味 | 典型例 | 要件 |
|---|---|---|---|
| `X` | 破壊的変更を含むmajor | public protocolの大規模破壊, repository構造の全面変更 | 当面は `0` のまま. `v1.0.0` は想定しない |
| `Y` | それなりに大きな機能追加, milestone, module互換性cohort | 700E baseline完了, 700F e2e完了, sweep engine追加 | 同じ `Y` のmodule同士を互換とみなす |
| `Z` | 細かい変更 | bugfix, docs, tests, small refactor, benchmark改善 | public protocolやmodule boundaryを破壊しない |

当面は `v0.Y.Z` で開発する. `v0.Y.Z` は外部利用者に対して安定APIを保証するものではない. ただしrepository内の統合作業では, **同じ `Y` を持つmodule同士は互換である** というルールを採用する.

- project `v0.4.2` へ統合するmoduleは, 原則としてmodule versionも `v0.4.Z` であること.
- moduleの `Y` がprojectの `Y` と異なる場合, CIは互換性警告または失敗にする.
- `Z` だけの差は互換とみなす.
- `Y` が異なるmoduleを使う場合はadapterまたはmigration issueを作り, Manager Codexが明示承認する.
- `v0.Y.Z` の全範囲に対する広い互換性は考えない. 互換性判断は同一 `Y` cohortに限定する.

#### 3.8.3 version bump規則

version変更は必ずissue-firstで行う. `VERSION`, module `VERSION`, `versions.toml`, `CHANGELOG.md`, 関連docs, CI期待値を同じ作業で更新する.

| 変更 | bump | 例 |
|---|---|---|
| typo, docs only, test追加, local bugfix | `Z+1` | `v0.3.1` -> `v0.3.2` |
| 新機能, milestone達成, module public capability追加 | `Y+1`, `Z=0` | `v0.3.4` -> `v0.4.0` |
| 破壊的変更 | 原則 `Y+1` in v0系. 必要時のみ `X+1` | `v0.4.9` -> `v0.5.0` |
| v1安定化 | まだ行わない | `v1.0.0` は当面禁止 |

pre-1.0では破壊的変更も `Y` bumpで扱ってよい. ただし `docs/protocol.md`, `docs/module_contracts.md`, `docs/changes/CR-####.md`, `versions.toml` を必ず更新し, module compatibility cohortを切り替える.

#### 3.8.4 module version descriptor

各moduleはruntime descriptorにversion情報を出す.

```cpp
struct ModuleVersionDescriptor {
    std::string module_name;
    std::string module_version;       // e.g. v0.4.2.
    std::string protocol_version;     // e.g. f700f-protocol-0.4.
    std::string plugin_abi_version;   // e.g. f700f-plugin-abi-0.1.
    int compatibility_minor;          // Y value from v0.Y.Z.
    bool experimental;
};
```

`f700f-info --version --json` はproject version, module versions, protocol version, plugin ABI version, git commit, build typeを出力する.

#### 3.8.5 Milestone plan

milestoneはproject versionの `Y` と対応させる. 下表は初期計画であり, Manager Codexは `docs/milestones.md` に実績と差分を記録する.

| Milestone | Project version | 目的 | 主な完了条件 | 主要module |
|---|---|---|---|---|
| M0 Bootstrap | `v0.0.1` | 指示書, repository方針, docs/issues/reports/version/TDD/CI設計を置く | master instruction, VERSION, versions.toml, issue template, report templateがある | Manager |
| M1 Foundation | `v0.1.0` | repository骨格, AGENTS, CMake, GitHub CI, TDD harness | CIでconfigure/build/unit smoke testが通る | 1, 2, 16 |
| M2 Protocol Core | `v0.2.0` | core data model, protocol, plugin ABI, version descriptor | public interfacesとmock pluginがtest可能 | 2, 3 |
| M3 Signal Basics | `v0.3.0` | audio IO, dataset, basic channel, SSB baseline | WAV/CF32入出力, AWGN, SSB standard/narrow比較 | 4, 8, 10 |
| M4 FreeDV References | `v0.4.0` | Codec adapter, FEC, OFDM, 700D/700E emulation/reference path | 700D/700E emulatedとoptional official adapter比較 | 5, 6, 7, 10 |
| M5 Pipeline Metrics | `v0.5.0` | end-to-end runner, metrics, report生成 | single-run JSONL/WAV/summary report出力 | 11, 12, 15 |
| M6 First 700F | `v0.6.0` | 700F-A/B/Cの初期end-to-end | 小規模条件で700F candidatesが走る | 5, 6, 7, 8, 11, 12 |
| M7 Sweep | `v0.7.0` | parameter sweep, Pareto, top-k抽出 | SSB/700D/700E/700Fを同一seedで比較しtop candidatesを出す | 13, 15 |
| M8 Validation | `v0.8.0` | golden/regression, official baseline validation, reproducibility | governance CI, regression, baseline reportが揃う | 10, 12, 16 |
| M9 Research Snapshot | `v0.9.0` | 初期研究snapshot, audit/security/bugcheck | 監査report,既知制限,次期計画が整理される | all |

`v1.0.0` は当面想定しない. M9以降も `v0.10.0`, `v0.11.0` のように継続してよい.

#### 3.8.6 TDD方針

原則は red -> green -> refactor とする.

1. issueにAcceptance criteriaとTest planを書く.
2. 可能な限り, 失敗するunit testまたはintegration smoke testを先に追加する.
3. 最小実装でtestをgreenにする.
4. refactorする.
5. deterministic seed, golden vector, boundary testを追加する.
6. issueに実行したtest commandと結果を記録する.

TDDの例外を許す条件:

- 外部Codec2/FreeDV APIの調査spike.
- 音声品質評価など, 先に人間が妥当性を聞く必要がある探索.
- CIやbuild systemのbootstrapで, test harness自体がまだ存在しない場合.

例外時も, issueに `TDD exception` と理由を書く. close前にはcharacterization test, regression test, またはgolden testを追加する.

各moduleの `docs/testing.md` には, そのmoduleで先に書くべきtest種類を明記する.

| Test種別 | 用途 |
|---|---|
| Unit test | module内の純粋関数, parser, descriptor, math helper |
| Property-like deterministic test | seed固定のchannel, random config生成, invariants |
| Golden test | 小さい既知入力に対する期待出力 |
| Integration smoke test | SSB/700D/700E/700Fの最小pipeline |
| Regression test | 発見済みbugの再発防止 |
| Governance test | issues/README, reports/README, versions.toml, docs indexの整合性 |

#### 3.8.7 GitHub CI方針

GitHub Actionsを使い, PR/pushで軽量CI, schedule/manualで重いsweepを実行する.

| Workflow | Trigger | 内容 | 失敗時の扱い |
|---|---|---|---|
| `.github/workflows/ci.yml` | pull_request, push | CMake configure, build, unit tests, integration smoke tests | merge禁止 |
| `.github/workflows/governance.yml` | pull_request, push | issue-first整合性, README index, version manifest, docs links | merge禁止 |
| `.github/workflows/security.yml` | pull_request, schedule | dependency/license/security/static checks | P0/P1ならmerge禁止 |
| `.github/workflows/nightly-sweep.yml` | schedule, workflow_dispatch | 代表sweep, artifact upload, report生成 | 失敗時issue化 |
| `.github/workflows/release-candidate.yml` | workflow_dispatch | milestone report, package, reproducibility check | manual判断 |

PR CIは重くしすぎない. 700F探索の本格sweepは `nightly-sweep.yml` またはmanual dispatchに回す. CI artifactには, test log, small WAV sample, metrics JSONL, summary markdown, reportsを含めてよい. 大きな音声datasetはrepositoryに入れず, manifestまたは生成scriptで扱う.

CIは少なくとも次を検査する.

- C++20 build on Linux. 可能ならWindowsもmatrixに含める.
- Debug build unit tests.
- Release build smoke test.
- `ctest --output-on-failure`.
- Python governance scripts.
- `VERSION`, `versions.toml`, module `VERSION` の整合性.
- issue fileと `issues/README.md` の整合性.
- report fileと `reports/README.md` の整合性.
- public interface変更時にdocs/protocol.mdが更新されているかの簡易check.

CI scriptは可能な限りlocalでも実行できるようにする.

```text
tools/check_governance.py
tools/check_versions.py
tools/run_ci_local.sh
```


## 4. システムを構成する16モジュール

全体を16モジュールに分ける. 各モジュールは個別Codexに分担可能であり, 最後に統合する.

1. Repository, Build, CI Foundation
2. Core Data Model and Protocol
3. Plugin ABI and Module Registry
4. Audio IO and Dataset Manager
5. Codec Adapter Layer
6. FEC and UEP Engine
7. OFDM Modem and Waveform Engine
8. HF Channel Simulator
9. Synchronization and Impairment Estimator
10. Reference Baselines: SSB, 700D, 700E
11. End-to-End Pipeline Runner
12. Metrics and Scoring Engine
13. Sweep and Optimization Engine
14. AI Integration Layer
15. Reporting and Visualization
16. Golden Tests, Benchmark, Integration QA

各モジュールの境界は, セクション5以降のprotocolに従うこと.

### 4.1 モジュールディレクトリ対応表

各moduleは必ず専用サブディレクトリを持つ. Module Codexはそのディレクトリを作業ルートとして起動される想定にする.

| Module | Directory | Primary owner | Notes |
|---:|---|---|---|
| 1 | `modules/01_repository_build_ci/` | Build Codex | root CMake連携はManager承認が必要. |
| 2 | `modules/02_core_data_model_protocol/` | Protocol Codex | すべての境界型の源泉. 破壊的変更は禁止. |
| 3 | `modules/03_plugin_abi_registry/` | ABI Codex | C ABI versioningとregistryを担当. |
| 4 | `modules/04_audio_io_dataset/` | Audio Codex | WAV/IQ/dataset manifestを担当. |
| 5 | `modules/05_codec_adapter/` | Codec Codex | Codec2 adapter, mock codec, future codec plugin. |
| 6 | `modules/06_fec_uep_engine/` | FEC Codex | LDPC, UEP, interleaveを担当. |
| 7 | `modules/07_ofdm_modem_waveform/` | Modem Codex | 700D/700E/700FのOFDM/QPSK波形とideal modemを担当. |
| 8 | `modules/08_hf_channel_simulator/` | Channel Codex | AWGN, fading, multipath, interferenceを担当. |
| 9 | `modules/09_sync_impairment_estimator/` | Sync Codex | CFO, timing, phase, resync metricsを担当. |
| 10 | `modules/10_reference_baselines_ssb_700d_700e/` | Baseline Codex | SSB, 700D, 700E emulation/reference comparisonを担当. |
| 11 | `modules/11_pipeline_runner/` | Pipeline Codex | end-to-end runnerとartifact generationを担当. |
| 12 | `modules/12_metrics_scoring/` | Metrics Codex | BER/FER/STOI/ASR WER/scoringを担当. |
| 13 | `modules/13_sweep_optimization/` | Sweep Codex | parameter sweep, Pareto探索を担当. |
| 14 | `modules/14_ai_integration/` | AI Codex | ONNX optional postprocess/evaluatorを担当. |
| 15 | `modules/15_reporting_visualization/` | Report Codex | report, plots, summary markdownを担当. |
| 16 | `modules/16_golden_benchmark_integration_qa/` | QA Codex | golden tests, benchmark, regressionを担当. |

各module directory内は, 少なくとも `AGENTS.md`, `README.md`, `CMakeLists.txt`, `include/`, `src/`, `tests/`, `docs/`, `issues/`, `reports/` を持つ. まだ実装がないmoduleも空directoryではなくstub targetと最小testを置く.

---

## 5. 共通データプロトコル

### 5.1 命名規約

- namespaceは `f700f` とする.
- ヘッダは `include/f700f/...` に置く.
- 実装は `src/...` に置く.
- public APIの型は `PascalCase`, 関数は `snake_case` または `lowerCamelCase` のどちらかに統一する. 最初の実装Codexが `docs/style.md` に決定を記録する.
- C ABIは `f700f_` prefixを必須にする.

### 5.2 基本型

C++内部では次の型を提供する.

```cpp
namespace f700f {

using SampleRateHz = uint32_t;
using Hz = double;
using Seconds = double;
using Decibels = double;
using Seed = uint64_t;

struct Complex32 {
    float re;
    float im;
};

struct AudioBlock {
    SampleRateHz sample_rate_hz;
    std::vector<float> mono;          // normalized [-1, 1].
    double start_time_s = 0.0;
};

struct ComplexBlock {
    SampleRateHz sample_rate_hz;
    std::vector<Complex32> iq;
    double center_frequency_hz = 0.0;
    double start_time_s = 0.0;
};

struct BitBlock {
    std::vector<uint8_t> bits;         // each element is 0 or 1.
    uint64_t frame_index = 0;
};

struct SoftBitBlock {
    std::vector<float> llr;            // positive means bit 1, negative means bit 0.
    uint64_t frame_index = 0;
};

struct FrameStatus {
    uint64_t frame_index = 0;
    bool sync = false;
    bool fec_ok = false;
    float confidence = 0.0f;
    uint32_t corrected_errors = 0;
    uint32_t erasures = 0;
};

}
```

### 5.3 Descriptor型

すべての差し替え可能モジュールはdescriptorを持つ.

```cpp
namespace f700f {

struct Capability {
    std::string key;
    std::string value;
};

struct ModuleDescriptor {
    std::string module_id;             // stable id, e.g. "ofdm_700e_compat".
    std::string module_name;
    std::string module_version;
    std::string abi_version;
    std::vector<Capability> capabilities;
};

struct ModeDescriptor {
    std::string mode_id;               // e.g. "700e_baseline", "700f_a".
    double rf_bandwidth_hz = 0.0;
    double audio_low_hz = 300.0;
    double audio_high_hz = 3300.0;
    double nominal_latency_s = 0.0;
    double frame_duration_s = 0.0;
    uint32_t raw_bitrate_bps = 0;
    uint32_t voice_bitrate_bps = 0;
    uint32_t text_bitrate_bps = 0;
    std::string codec_id;
    std::string fec_id;
    std::string modem_id;
};

}
```

### 5.4 ファイルプロトコル

| 拡張子 | 形式 | 用途 |
|---|---|---|
| `.wav` | PCM16またはfloat WAV | 入力音声, 復調音声, SSB比較音声 |
| `.cf32` | little-endian float32 interleaved IQ | ベースバンドIQ保存 |
| `.bits` | packed bits | 送信ビット列, 復号ビット列 |
| `.llr` | float32 LLR列 | soft decision保存 |
| `.jsonl` | 1行1JSON | 実験結果ログ |
| `.csv` | RFC4180相当 | 集計結果 |
| `.toml` | TOML | mode, channel, sweep config |

`.cf32` には必ず同名 `.toml` sidecarを付ける.

例: `run_000042.rx.cf32` と `run_000042.rx.cf32.toml`.

---

## 6. C++内部Interface

### 6.1 IAudioCodec

```cpp
class IAudioCodec {
public:
    virtual ~IAudioCodec() = default;
    virtual ModuleDescriptor descriptor() const = 0;
    virtual SampleRateHz input_sample_rate() const = 0;
    virtual SampleRateHz output_sample_rate() const = 0;
    virtual Seconds frame_duration() const = 0;
    virtual uint32_t bits_per_frame() const = 0;
    virtual BitBlock encode(const AudioBlock& audio) = 0;
    virtual AudioBlock decode(const BitBlock& bits, const FrameStatus& status) = 0;
};
```

要件:
- 既存Codec2接続用adapterを実装する.
- ダミーcodecとしてPCM量子化codecを実装し, テストに使えるようにする.
- 将来のニューラルcodecをC ABIまたはONNX経由で追加できるようにする.

### 6.2 IFecCodec

```cpp
class IFecCodec {
public:
    virtual ~IFecCodec() = default;
    virtual ModuleDescriptor descriptor() const = 0;
    virtual uint32_t input_bits_per_frame() const = 0;
    virtual uint32_t output_bits_per_frame() const = 0;
    virtual BitBlock encode(const BitBlock& payload) = 0;
    virtual std::pair<BitBlock, FrameStatus> decode(const SoftBitBlock& soft_bits) = 0;
};
```

要件:
- no-FEC baselineを必ず用意する.
- repetition codeをテスト用に用意する.
- LDPC adapterを設計する.
- UEPは, payloadをpriority classに分ける共通protocolを用意する.

### 6.3 IModem

```cpp
class IModem {
public:
    virtual ~IModem() = default;
    virtual ModuleDescriptor descriptor() const = 0;
    virtual ModeDescriptor mode() const = 0;
    virtual ComplexBlock modulate(const BitBlock& protected_bits) = 0;
    virtual std::pair<SoftBitBlock, FrameStatus> demodulate(const ComplexBlock& rx) = 0;
};
```

要件:
- 700E compatibility pathを最初の目標にする.
- 700F candidate pathを別実装として作る.
- modemはチャンネルを知らない. 入出力はComplexBlockのみ.
- occupied bandwidthを推定するユーティリティを別moduleとして提供する.

### 6.4 IChannelModel

```cpp
class IChannelModel {
public:
    virtual ~IChannelModel() = default;
    virtual ModuleDescriptor descriptor() const = 0;
    virtual ComplexBlock apply(const ComplexBlock& tx, uint64_t run_index) = 0;
};
```

要件:
- channelは完全に決定論的にする. config + seed + run_indexが同じなら同じ出力になること.
- 複数channelをchainできる `ChannelChain` を実装する.

### 6.5 IMetric

```cpp
class IMetric {
public:
    virtual ~IMetric() = default;
    virtual ModuleDescriptor descriptor() const = 0;
    virtual nlohmann::json evaluate(const EvaluationContext& ctx) = 0;
};
```

要件:
- C++側はBER, FER, sync loss, latency, RMS, SNR, EVM, occupied BWを実装する.
- Python側はSTOI, ASR WER, plotを扱ってよい.

### 6.6 IAudioPostProcessor

```cpp
class IAudioPostProcessor {
public:
    virtual ~IAudioPostProcessor() = default;
    virtual ModuleDescriptor descriptor() const = 0;
    virtual AudioBlock process(const AudioBlock& input, const nlohmann::json& params) = 0;
};
```

要件:
- 初期実装はidentityのみ.
- ONNX音声強調はoptional pluginにする.

---

## 7. C ABI Plugin Protocol

C ABIは将来のRust, Zig, C, C++, Python extension製モジュールとの接続点である. ABIは破壊的変更を避ける.

### 7.1 基本方針

- C ABIは低頻度呼び出し中心にする. 高頻度DSP sample処理は内部C++ interfaceでよい.
- pluginはdescriptorを返す.
- pluginは状態handleを生成し, destroyする.
- メモリ所有権はcallerまたはpluginのどちらかを明示する.
- version mismatch時は明確なerror codeを返す.

### 7.2 ABI version

初期ABI versionは `f700f-plugin-abi-0.1` とする.

### 7.3 C ABI概略

```c
#define F700F_PLUGIN_ABI_VERSION 1

typedef struct f700f_status {
    int32_t code;
    const char* message;
} f700f_status;

typedef struct f700f_module_descriptor {
    const char* module_id;
    const char* module_name;
    const char* module_version;
    const char* abi_version;
} f700f_module_descriptor;

typedef struct f700f_buffer_f32 {
    float* data;
    uint64_t size;
} f700f_buffer_f32;

typedef struct f700f_buffer_u8 {
    uint8_t* data;
    uint64_t size;
} f700f_buffer_u8;

typedef void* f700f_plugin_handle;

typedef struct f700f_plugin_vtable {
    uint32_t abi_version;
    f700f_status (*get_descriptor)(f700f_module_descriptor* out_desc);
    f700f_status (*create)(const char* json_config, f700f_plugin_handle* out_handle);
    f700f_status (*destroy)(f700f_plugin_handle handle);
} f700f_plugin_vtable;

f700f_status f700f_get_plugin_vtable(f700f_plugin_vtable* out_vtable);
```

各種codec, fec, modem, channelの専用ABIは, 上記共通vtableに機能別vtableを追加する形にする.

---

## 8. 設定ファイル仕様

### 8.1 Mode config例

```toml
[mode]
mode_id = "700f_a"
rf_bandwidth_hz = 1900.0
audio_low_hz = 300.0
audio_high_hz = 3300.0
frame_duration_ms = 80
nominal_latency_ms = 850

[codec]
codec_id = "codec2_1300_adapter"
voice_bitrate_bps = 1300
sample_rate_hz = 8000

[fec]
fec_id = "uep_ldpc_v1"
rate = 0.55
interleaver_depth_frames = 4
priority_classes = 3

[modem]
modem_id = "ofdm_qpsk_700f_candidate"
carrier_count = 27
cyclic_prefix_ms = 6
pilot_scheme = "700e_like"
soft_decision = true

[text]
enabled = true
bitrate_bps = 25
```

### 8.2 Channel config例

```toml
[channel]
channel_id = "hf_moderate"
seed = 123456789

[[channel.chain]]
type = "frequency_offset"
offset_hz = 50.0

[[channel.chain]]
type = "sample_clock_offset"
ppm = 10.0

[[channel.chain]]
type = "multipath_rayleigh"
delay_spread_ms = 3.0
doppler_hz = 1.0
path_count = 4

[[channel.chain]]
type = "awgn"
snr_db = 2.0

[[channel.chain]]
type = "impulse_noise"
probability_per_second = 0.2
mean_duration_ms = 15.0
power_db_over_signal = 12.0
```

### 8.3 Sweep config例

```toml
[sweep]
name = "initial_700f_sweep"
repetitions = 5
base_seed = 20260227
parallel_jobs = 8

[modes]
ids = ["700e_baseline", "700f_a", "700f_b", "700f_c", "ssb_baseline"]

[dataset]
manifest = "datasets/speech_manifest.example.toml"
max_seconds_per_case = 30

[grid]
snr_db = [-4, -2, 0, 2, 4, 6, 8]
doppler_hz = [0.5, 1.0, 2.0, 4.0]
delay_spread_ms = [1.0, 2.0, 4.0, 6.0]
freq_offset_hz = [0, 50, 100, 200]
```

---

## 9. Channel model仕様

### 9.1 必須channel

| Channel | 必須パラメータ | 目的 |
|---|---|---|
| AWGN | snr_db, measured_bandwidth_hz | 純粋なSNR耐性 |
| Frequency offset | offset_hz | 周波数ずれ耐性 |
| Sample clock offset | ppm | サウンドカード/無線機のクロック誤差 |
| Flat Rayleigh fading | doppler_hz | QSB耐性 |
| Multipath Rayleigh | path delays, path powers, doppler_hz | 選択性フェージング |
| Notch fading | center_hz, width_hz, depth_db, time_variation | 帯域内の一部落ち込み |
| Impulse noise | probability, duration, power | QRN, 静電ノイズ |
| Adjacent interference | waveform_type, offset_hz, power_db | 隣接SSB/CW/デジタル混信 |
| Band limiting | low_hz, high_hz, filter_order | 実機IF/filterの近似 |

### 9.2 SNR定義

SNRは, 原則として受信側が見るモード占有帯域内の信号電力対雑音電力で定義する. ただしSSB比較では, SSB音声帯域または指定IF帯域でのSNRも出力する.

すべての結果ログに次を含めること.

- `snr_definition`
- `measurement_bandwidth_hz`
- `signal_power_dbfs`
- `noise_power_dbfs`

### 9.3 フェージング再現性

Rayleigh/Rician/multipath modelは, seed, run_index, sample_rate, durationが同じなら完全に同じ系列を生成すること.

---

## 10. SSB Baseline仕様

SSB baselineは700F評価の基準である. デジタル系と同じ入力音声, 同じチャンネル条件で比較可能にする.

### 10.1 基本処理

```text
input speech
-> bandpass 300-2700 Hz or 300-3000 Hz
-> optional compressor/limiter
-> analytic signal generation
-> SSB modulation to complex baseband
-> channel
-> SSB demodulation
-> audio bandpass
-> AGC/level normalization
-> output wav
```

### 10.2 SSB profiles

| profile | 音声帯域 | 用途 |
|---|---:|---|
| ssb_standard_3k | 300-2900 Hz | 通常SSB基準 |
| ssb_narrow_1p9k | 300-1950 Hz程度 | 700F同帯域比較 |
| ssb_communications | 300-2700 Hz + compressor | 実用通信音基準 |

### 10.3 比較時の注意

- 700Fは1.9 kHz占有, SSB standardは3 kHz占有で比較してよい.
- ただし公平性確認として, SSB narrow 1.9 kHzも必ず比較する.
- SSBの評価はBERではなく, 音声指標とASR WERを中心にする.

---

## 10A. SSB, 700D, 700E Baseline/Emulation仕様

700F探索では, SSBだけでなく, 既存FreeDV 700Dおよび700E相当を必ず同じ条件で比較する. ここでいうemulationは2段階に分ける.

| Level | 名称 | 内容 | 目的 |
|---:|---|---|---|
| 0 | Spec-level emulation | 文献/Codec2 sourceで確認したcarrier数, bandwidth, raw bitrate, FEC, pilot/CP仮定に基づく独立C++モデル | 高速sweep, 700F候補との公平な構成比較 |
| 1 | Reference adapter | 実際のCodec2/FreeDV `freedv_tx`, `freedv_rx`, FreeDV API, または該当CLIを呼び出すadapter | 既存実装との差分確認 |
| 2 | Golden cross-check | 公式/自前golden vectorで, Level 0/1の傾向とmetricを検証 | 退行検出, 実装信頼性確保 |

Level 0は高速探索用であり, 公式FreeDV実装そのものではない. Level 1/2でCodec2/FreeDV実装と比較し, 差分を `docs/codec2_verification_notes.md` に記録すること.

### 10A.1 Reference mode descriptor

`ModeDescriptor` は, `mode_family` と `emulation_level` を必ず持つ.

```toml
[mode]
name = "700e_baseline"
mode_family = "freedv_reference"
reference_mode = "700E"
emulation_level = 0
rf_bandwidth_hz = 1500
voice_bandwidth_hz = 3000
sample_rate_hz = 8000

[codec]
name = "codec2_700c_adapter"
bitrate_bps = 700

[modem]
name = "coherent_ofdm_qpsk"
carrier_count = 21
raw_bitrate_bps = 3000

[fec]
name = "ldpc"
code = "LDPC_112_56"

[text]
bitrate_bps = 25
```

### 10A.2 700D baseline初期値

初期値は, 作業開始時にCodec2/FreeDV sourceで再検証する. 検証前の設計仮説として次を置く.

| 項目 | 初期値 |
|---|---:|
| reference_mode | `700D` |
| codec | Codec2 700C相当 |
| modem | 17 carrier coherent OFDM/QPSK相当 |
| RF BW | 1000 Hz |
| raw bits/s | 1900 bit/s |
| FEC | LDPC(224,112)相当 |
| text bits/s | 25 bit/s |
| AWGN SNR min reference | -2 dB目安 |
| multipath reference | fair目安 |
| 設計上の性格 | 低SNR重視, 強FEC, fast fadingでは不利になりうる |

`700d_baseline.toml` には, Level 0/1の両方を切り替えられる設定を置く. Level 1が利用不可の場合, CIではLevel 0だけを実行してよいが, nightly/local full testではLevel 1の実行を目標にする.

### 10A.3 700E baseline初期値

初期値は, 作業開始時にCodec2/FreeDV sourceで再検証する. 検証前の設計仮説として次を置く.

| 項目 | 初期値 |
|---|---:|
| reference_mode | `700E` |
| codec | Codec2 700C相当 |
| modem | 21 carrier coherent OFDM/QPSK相当 |
| RF BW | 1500 Hz |
| raw bits/s | 3000 bit/s |
| FEC | LDPC(112,56)相当 |
| text bits/s | 25 bit/s |
| AWGN SNR min reference | 1 dB目安 |
| multipath reference | good目安 |
| 設計上の性格 | 700Dより広帯域, fast fading/multipath寄り, 700Fの直接ベースライン |

`700e_baseline.toml` には, Level 0/1の両方を切り替えられる設定を置く.

### 10A.4 SSB emulation初期値

SSBはLevel 0のみでよい. ただし, `ssb_standard_3k`, `ssb_narrow_1p9k`, `ssb_communications` の3 profileを必須にする.

```text
input speech
-> pre-emphasis optional
-> bandpass/compressor according to profile
-> analytic signal
-> USB/LSB complex baseband modulation
-> common channel model
-> product detector / envelope-equivalent demodulation
-> AGC/normalization
-> output wav + metrics
```

700Fの主比較は次の2種類を必ず出す.

1. `700F 1.9kHz` vs `SSB standard 3kHz`: 実運用上の置換価値.
2. `700F 1.9kHz` vs `SSB narrow 1.9kHz`: 同占有帯域での公平比較.

### 10A.5 Baseline sweep必須条件

`reference_baseline_sweep.toml` は最低限次を含む.

| baseline | SNR dB | channel |
|---|---|---|
| SSB standard 3k | -4, -2, 0, 2, 4, 6, 8 | AWGN, hf_mild, hf_moderate, hf_severe |
| SSB narrow 1.9k | -4, -2, 0, 2, 4, 6, 8 | AWGN, hf_mild, hf_moderate, hf_severe |
| 700D Level 0 | -4, -2, 0, 2, 4, 6, 8 | AWGN, hf_mild, hf_moderate, hf_severe, dx_selective |
| 700E Level 0 | -4, -2, 0, 2, 4, 6, 8 | AWGN, hf_mild, hf_moderate, hf_severe, dx_selective |
| 700D Level 1 | selected subset | AWGN, hf_moderate |
| 700E Level 1 | selected subset | AWGN, hf_moderate |

Level 1は外部Codec2 buildに依存するため, CI必須にはしない. ただしManager Codexは, Level 1が利用可能な環境では自動的に有効化されるCMake optionとruntime detectionを用意する.

### 10A.6 700Fとの比較出力

すべてのbaselineと700F candidateは, 同じ `RunManifest` に次を出す.

- `mode_name`
- `mode_family`
- `reference_mode`
- `emulation_level`
- `channel_config_hash`
- `seed`
- `snr_db`
- `audio_in_hash`
- `audio_out_path`
- `iq_tx_path` optional
- `iq_rx_path` optional
- `metric_json_path`
- `timing_json_path`

スコアリング時には, `700F-A/B/C` が `SSB standard`, `SSB narrow`, `700D`, `700E` のどれに対して勝ったかを別々に集計する. 単一の総合点だけで判断しない.

---

## 11. Metrics and Scoring仕様

### 11.1 必須metrics

| Metric | 対象 | 出力 |
|---|---|---|
| BER | digital | bit_error_rate |
| FER | digital | frame_error_rate |
| Sync loss rate | digital | sync_loss_rate |
| Resync time | digital | mean_resync_time_s |
| Dropout rate | audio | dropout_rate |
| Latency | all | effective_latency_s |
| Occupied BW | waveform | occupied_bw_99_hz, occupied_bw_occupied_hz |
| EVM | modem | evm_db |
| Output SNR | audio | audio_snr_db where reference exists |
| Level stats | audio | rms_dbfs, peak_dbfs, clipping_count |
| STOI/ESTOI optional | audio | stoi, estoi |
| ASR WER optional | audio | wer, cer |
| Human test export | audio | ABX package manifest |

### 11.2 スコア関数

初期スコアは次の形にする.

```text
score =
  + 0.35 * intelligibility_score
  + 0.20 * asr_score
  + 0.15 * robustness_score
  + 0.10 * audio_quality_score
  + 0.10 * bandwidth_score
  + 0.05 * latency_score
  + 0.05 * sync_stability_score
```

各subscoreは0.0から1.0に正規化する. 正規化定義は `docs/scoring_method.md` に明記する.

### 11.3 Pareto分析

単一スコアだけでなく, 次の軸のPareto frontierを出す.

- 明瞭度 vs 必要SNR
- 明瞭度 vs 遅延
- 明瞭度 vs 占有帯域幅
- 音声品質 vs フェージング耐性
- 700Eとの差分 vs SSBとの差分

---

## 12. 700F候補仕様

初期候補として最低3種類を実装する. 実装後に探索で自動生成候補を増やす.

### 12.1 700F-A: Balanced SSB replacement

| 項目 | 値 |
|---|---:|
| RF BW | 1900 Hz |
| Audio BW | 300-3300 Hz |
| Frame | 80 ms |
| CP | 6 ms |
| Codec | 1300 bps級またはCodec2 adapter候補 |
| FEC | UEP medium |
| Interleaver | 4 frames |
| 目的 | 平均的QSOでSSBより聞きやすい領域を狙う |

### 12.2 700F-B: Robust fading mode

| 項目 | 値 |
|---|---:|
| RF BW | 1900 Hz |
| Audio BW | 300-3000 Hz |
| Frame | 100 ms or 120 ms |
| CP | 8 ms |
| Codec | 700C/1200/1300候補 |
| FEC | UEP strong |
| Interleaver | 6 to 8 frames |
| 目的 | 700Eより少し音声品質を上げつつ悪条件寄り |

### 12.3 700F-C: Quality leaning mode

| 項目 | 値 |
|---|---:|
| RF BW | 1900 Hz |
| Audio BW | 300-3400 Hz |
| Frame | 80 ms |
| CP | 6 ms |
| Codec | 1600 bps級 |
| FEC | UEP light/medium |
| Interleaver | 2 to 4 frames |
| 目的 | 良条件から中条件でSSBより自然に聞こえる領域を狙う |

### 12.4 700F探索範囲

探索対象は次とする.

| パラメータ | 候補 |
|---|---|
| RF BW | 1750, 1800, 1850, 1900, 1950 Hz |
| Audio high | 2700, 3000, 3300, 3600 Hz |
| Frame | 80, 100, 120, 160 ms |
| CP | 4, 6, 8 ms |
| Carrier count | 23, 25, 27, 29, 31 |
| FEC rate | 0.45, 0.50, 0.55, 0.60, 0.67 |
| Interleaver depth | 1, 2, 4, 6, 8 frames |
| Pilot density | 700E-like, high, medium |
| Codec bitrate | 700, 1200, 1300, 1600, 2400 if feasible |

探索は最初から全組み合わせにしない. まずLatin hypercubeまたはrandom searchで粗く探索し, 上位候補をgrid refinementする.

---

## 13. 各モジュール詳細指示

以下, 各モジュールを担当するCodexへの個別指示を示す.

---

### Module 1: Repository, Build, CI Foundation

#### 目的

リポジトリ骨格, CMake, テスト基盤, format, lint, dependency導入を整備する.

#### 実装物

- Top-level `CMakeLists.txt`.
- `cmake/Dependencies.cmake`.
- `VERSION`.
- `versions.toml`.
- `CHANGELOG.md`.
- `.gitmodules` initial policy.
- `docs/git_workflow.md`.
- `docs/submodule_policy.md`.
- `docs/coordination/branches.md`.
- `include/f700f/version.hpp`.
- `src/apps/f700f-info.cpp`.
- `tests/unit/test_version.cpp`.
- `tools/check_versions.py`.
- `tools/check_governance.py`.
- `tools/check_submodules.py`.
- `tools/check_branch_issue.py`.
- `.github/workflows/ci.yml`.
- `.github/workflows/governance.yml`.
- `.github/workflows/nightly-sweep.yml`.
- `.clang-format`.
- `.clang-tidy`.
- `python/pyproject.toml`.
- `docs/build.md`.

#### 要件

- C++20を要求する.
- Debug, Release, RelWithDebInfoをサポートする.
- `f700f-info --version` が動く.
- `f700f-info --version --json` がproject/module/protocol/ABI versionを出す.
- CTestでunit testが実行できる.
- GitHub Actions workflowが軽量CIを実行する.
- `develop` を統合branchとし, issue単位branchからPR/mergeするGit Flow運用をdocsに書く.
- branch名にissue IDが含まれることを検査できる.
- 外部Codec2はgit submoduleとしてpinする方針にする. ただし最初はoptionalにし, 見つからない場合でもcore testsは通す.
- submoduleがdirtyでないこと, pointer変更に対応issueがあることを検査できる.

#### Acceptance criteria

- `cmake -S . -B build -DCMAKE_BUILD_TYPE=Release` が通る.
- `cmake --build build` が通る.
- `ctest --test-dir build` が通る.
- `f700f-info --version` がJSONでversion情報を出す.
- `tools/check_versions.py` がproject/module version整合性を検査できる.
- GitHub ActionsのCI定義が存在し, local commandと同等のbuild/testを実行する.
- `docs/git_workflow.md`, `docs/submodule_policy.md`, `docs/coordination/branches.md` が存在する.
- `tools/check_branch_issue.py` がbranch名とissue IDの対応を検査できる.
- `tools/check_submodules.py` が `.gitmodules`, submodule pointer, dirty stateを検査できる.

---

### Module 2: Core Data Model and Protocol

#### 目的

共通型, descriptor, config model, JSON/TOML変換, file manifestを実装する.

#### 実装物

- `include/f700f/core/types.hpp`.
- `include/f700f/core/result.hpp`.
- `include/f700f/protocol/descriptors.hpp`.
- `include/f700f/protocol/config.hpp`.
- `src/protocol/config.cpp`.
- `tests/unit/test_protocol_types.cpp`.
- `docs/protocol.md`.

#### 要件

- すべての基本型にvalidate関数を用意する.
- config load時に範囲チェックをする.
- config hashを生成できる.
- JSON serializationを用意する.
- TOML readerは外部ライブラリ使用可.

#### Acceptance criteria

- mode config例を読み込み, ModeDescriptorへ変換できる.
- channel config例を読み込み, JSONへ変換できる.
- 不正値に対し明確なerrorを返す.

---

### Module 3: Plugin ABI and Module Registry

#### 目的

差し替え可能なmodule registryとC ABI plugin loaderを作る.

#### 実装物

- `include/f700f/plugin/abi.h`.
- `include/f700f/plugin/module_registry.hpp`.
- `src/plugin/module_registry.cpp`.
- `src/plugin/dynamic_loader.cpp`.
- `tests/unit/test_module_registry.cpp`.
- `tests/integration/plugin_dummy/`.
- `docs/plugin_abi.md`.

#### 要件

- static built-in module登録.
- dynamic library plugin登録.
- descriptor照会.
- version mismatch検出.
- pluginロード失敗時の安全なerror処理.

#### Acceptance criteria

- dummy pluginをロードできる.
- ABI mismatch dummy pluginを拒否できる.
- module_id重複を検出できる.

---

### Module 4: Audio IO and Dataset Manager

#### 目的

音声入出力, resampling, bandpass, speech dataset manifestを扱う.

#### 実装物

- `include/f700f/audio/wav_io.hpp`.
- `include/f700f/audio/resampler.hpp`.
- `include/f700f/audio/filter.hpp`.
- `include/f700f/audio/dataset.hpp`.
- `src/audio/*.cpp`.
- `tests/unit/test_wav_io.cpp`.
- `tests/unit/test_audio_filter.cpp`.
- `datasets/speech_manifest.example.toml`.

#### 要件

- WAV PCM16/PCM32F読み書き.
- mono化.
- 8 kHz, 16 kHz, 48 kHz間のresample.
- 300-3300 Hz等のbandpass.
- 音量正規化.
- dataset manifestから発話セットを列挙.

#### Acceptance criteria

- WAV round-trip誤差が許容値内.
- bandpassの周波数応答をunit testで確認.
- 30秒以下の音声chunkを自動切り出しできる.

---

### Module 5: Codec Adapter Layer

#### 目的

Codec2/FreeDVのcodec機能と, 将来codecを抽象化するadapter層を作る.

#### 実装物

- `include/f700f/codec/audio_codec.hpp`.
- `include/f700f/codec/codec2_adapter.hpp`.
- `include/f700f/codec/dummy_codecs.hpp`.
- `src/codec/*.cpp`.
- `tests/unit/test_dummy_codec.cpp`.
- `tests/integration/test_codec2_adapter.cpp` if Codec2 available.
- `docs/codec_adapter.md`.

#### 要件

- `IAudioCodec` interfaceを実装する.
- dummy codecを必ず用意する.
- Codec2 adapterはoptional buildにする.
- Codec2の実際のframe duration, bits per frameをソースから取得またはadapter内に記録し, docsへ出す.
- codec outputはBitBlockであり, FECに渡せること.

#### Acceptance criteria

- dummy codecでencode/decode round-tripが動く.
- Codec2がある場合, 1秒WAVをencode/decodeしWAV出力できる.
- codec descriptorが正しい.

---

### Module 6: FEC and UEP Engine

#### 目的

FEC抽象化, UEP priority mapping, no-FEC, repetition, LDPC adapterを実装する.

#### 実装物

- `include/f700f/fec/fec_codec.hpp`.
- `include/f700f/fec/priority_map.hpp`.
- `include/f700f/fec/no_fec.hpp`.
- `include/f700f/fec/repetition_fec.hpp`.
- `include/f700f/fec/ldpc_adapter.hpp`.
- `src/fec/*.cpp`.
- `tests/unit/test_repetition_fec.cpp`.
- `tests/unit/test_priority_map.cpp`.

#### 要件

- SoftBitBlock入力でdecodeする.
- hard bitsからLLRを生成するutilityを用意する.
- UEPはpriority classごとにFEC強度を変えられる設計にする.
- LDPC本体が未実装の場合も, adapter stubとrepetitionでpipelineを動かす.

#### Acceptance criteria

- repetition FECでAWGN soft inputから訂正効果を確認できる.
- priority_mapで重要bitが強保護側に入る.
- no-FECとFECの差をmetricで出せる.

---

### Module 7: OFDM Modem and Waveform Engine

#### 目的

OFDM/QPSK modemを抽象化し, 700E baselineと700F candidatesを実験できる波形engineを作る.

#### 実装物

- `include/f700f/modem/modem.hpp`.
- `include/f700f/modem/ofdm_config.hpp`.
- `include/f700f/modem/ofdm_modem.hpp`.
- `src/modem/*.cpp`.
- `tests/unit/test_qpsk_mapping.cpp`.
- `tests/unit/test_ofdm_no_channel.cpp`.
- `tests/integration/test_modem_awgn.cpp`.
- `docs/ofdm_modem.md`.

#### 要件

- QPSK mapping/demapping.
- OFDM symbol generation.
- Cyclic Prefix configurable.
- carrier count configurable.
- pilot scheme configurable.
- soft decision demapping.
- frame index tracking.
- occupied bandwidth measurement helper.

#### 700E互換について

初期実装では, Codec2本体のFreeDV APIを直接使う700E pipelineと, 独自OFDM modemの700E-like pipelineを分けてよい.

最重要なのは, 700F探索でパラメータを自由に振れること. 完全な700E bit-exact互換は第2優先とする.

#### Acceptance criteria

- no-channelでBER=0になる.
- AWGNでSNR上昇に従いBERが下がる.
- carrier count, CP, pilot設定をconfigで変えられる.
- `.cf32`へtx waveformを書ける.

---

### Module 8: HF Channel Simulator

#### 目的

HF劣化を可変にしたチャンネルシミュレータを作る.

#### 実装物

- `include/f700f/channel/channel_model.hpp`.
- `include/f700f/channel/awgn.hpp`.
- `include/f700f/channel/fading.hpp`.
- `include/f700f/channel/multipath.hpp`.
- `include/f700f/channel/frequency_offset.hpp`.
- `include/f700f/channel/sample_clock_offset.hpp`.
- `include/f700f/channel/impulse_noise.hpp`.
- `include/f700f/channel/adjacent_interference.hpp`.
- `src/channel/*.cpp`.
- `tests/unit/test_awgn_power.cpp`.
- `tests/unit/test_frequency_offset.cpp`.
- `tests/unit/test_channel_determinism.cpp`.

#### 要件

- ChannelChainで複数劣化を順に適用できる.
- seedにより完全再現できる.
- AWGN SNR定義を明記する.
- multipath fadingは時間変動を持てる.
- notch fadingは選択性フェージングの近似として実装する.

#### Acceptance criteria

- 同じseedで同じ出力.
- AWGNの実測SNRが指定値±0.2 dB以内.
- frequency offsetの推定値が指定値に近い.
- multipathでスペクトルにnotchが出る.

---

### Module 9: Synchronization and Impairment Estimator

#### 目的

復調前後の同期状態, 周波数ずれ, SNR, EVM等を推定する補助層を作る.

#### 実装物

- `include/f700f/sync/impairment_estimator.hpp`.
- `include/f700f/sync/snr_estimator.hpp`.
- `include/f700f/sync/frequency_offset_estimator.hpp`.
- `include/f700f/sync/sync_tracker.hpp`.
- `src/sync/*.cpp`.
- `tests/unit/test_snr_estimator.cpp`.
- `tests/unit/test_freq_estimator.cpp`.

#### 要件

- modemとは独立した測定器として動く.
- sync loss/resync timeを出す.
- estimator結果をmetricsに渡す.

#### Acceptance criteria

- AWGNでSNR推定値が近い.
- 既知frequency offsetを推定できる.
- sync状態の時系列JSONを出力できる.

---

### Module 10: Reference Baselines: SSB, 700D, 700E

#### 目的

700F候補を評価するための比較対象を実装する. ここではSSBだけではなく, FreeDV 700Dおよび700E相当のLevel 0 emulationと, Codec2/FreeDV実装を呼び出すLevel 1 reference adapterを担当する.

#### 担当directory

- `modules/10_reference_baselines_ssb_700d_700e/`

#### 実装対象

- `modules/10_reference_baselines_ssb_700d_700e/include/f700f/reference/reference_baseline.hpp`.
- `modules/10_reference_baselines_ssb_700d_700e/src/ssb_baseline.cpp`.
- `modules/10_reference_baselines_ssb_700d_700e/src/freedv_reference_emulator.cpp`.
- `modules/10_reference_baselines_ssb_700d_700e/src/freedv_external_adapter.cpp`.
- `modules/10_reference_baselines_ssb_700d_700e/src/reference_mode_factory.cpp`.
- `configs/modes/ssb_standard_3k.toml`.
- `configs/modes/ssb_narrow_1p9k.toml`.
- `configs/modes/700d_baseline.toml`.
- `configs/modes/700e_baseline.toml`.
- `tests/unit/test_ssb_baseline.cpp`.
- `tests/unit/test_reference_mode_descriptors.cpp`.
- `tests/integration/test_reference_baselines.cpp`.
- `docs/reference_baselines.md`.

#### 要件

- SSB baselineは, 300-2700Hz/300-2900Hz/狭帯域1.9kHz相当のprofileを持つ.
- 700D Level 0は, Codec2 700C相当payload, 17 carrier coherent OFDM/QPSK相当, RF BW 1000Hz, raw 1900bit/s, LDPC(224,112)相当を初期仮説にする.
- 700E Level 0は, Codec2 700C相当payload, 21 carrier coherent OFDM/QPSK相当, RF BW 1500Hz, raw 3000bit/s, LDPC(112,56)相当を初期仮説にする.
- Level 0は高速な独立C++ emulationであり, 公式FreeDV bit-exact互換を目標にしない.
- Level 1は外部Codec2/FreeDVの `freedv_tx`/`freedv_rx` またはFreeDV APIを呼び出すadapterとする.
- Level 1が利用不可の場合でもLevel 0でtestが通る.
- Level 0とLevel 1の差分を `docs/codec2_verification_notes.md` に記録する.
- すべてのbaselineは `IReferenceMode` または `IPipelineStage` としてpipelineへ接続できる.
- common channel modelを必ず通せるようにする.
- 出力は700F候補と同じartifact protocolに合わせる.

#### Acceptance criteria

- `f700f-sim --mode ssb_standard_3k`, `--mode ssb_narrow_1p9k`, `--mode 700d_baseline`, `--mode 700e_baseline` が動く.
- fixed seedでSSB, 700D, 700E baselineのartifact hashまたはmetricが安定する.
- `reference_baseline_sweep.toml` でSSB/700D/700Eの最低限のsweepが完走する.
- Level 1が環境に存在しない場合はskip理由を明示し, test失敗にしない.
- Level 1が環境に存在する場合は700D/700Eそれぞれ1条件以上で比較artifactを生成する.
- integration testで700F候補とSSB/700D/700Eの横並びreportが生成される.

### Module 11: End-to-End Pipeline Runner

#### 目的

音声入力から変調, channel, 復調, 音声出力, metricsまでを1 runとして実行する.

#### 実装物

- `include/f700f/pipeline/pipeline.hpp`.
- `src/pipeline/pipeline.cpp`.
- `src/apps/f700f-sim.cpp`.
- `src/apps/f700f-render.cpp`.
- `tests/integration/test_pipeline_dummy.cpp`.
- `docs/pipeline.md`.

#### 要件

- mode config + channel config + input wavで1 runを実行.
- tx bits, rx bits, tx waveform, rx waveform, output wav, metrics JSONを保存.
- digital modeとSSB modeを同じrunnerで扱う.
- run manifestを必ず出力.

#### CLI例

```bash
f700f-sim \
  --mode configs/modes/700f_a.toml \
  --channel configs/channels/hf_moderate.toml \
  --input datasets/sample.wav \
  --out reports/run_0001
```

#### Acceptance criteria

- dummy codec + no channelでend-to-endが通る.
- SSB baselineでend-to-endが通る.
- 700F candidateでend-to-endが通る.

---

### Module 12: Metrics and Scoring Engine

#### 目的

各runのmetric算出, summary, score化を行う.

#### 実装物

- `include/f700f/metrics/evaluation_context.hpp`.
- `include/f700f/metrics/ber.hpp`.
- `include/f700f/metrics/fer.hpp`.
- `include/f700f/metrics/audio_stats.hpp`.
- `include/f700f/metrics/occupied_bandwidth.hpp`.
- `include/f700f/metrics/scoring.hpp`.
- `src/metrics/*.cpp`.
- `python/f700f_lab/asr_wer.py`.
- `tests/unit/test_ber.cpp`.
- `tests/unit/test_scoring.cpp`.
- `docs/scoring_method.md`.

#### 要件

- C++で基本metricを出す.
- Python optionalでASR WER, STOI/ESTOIを追加できる.
- score関数はconfigで重みを変更可能.
- JSONLに追記できる.

#### Acceptance criteria

- 既知bit列でBERが正しい.
- occupied BWの推定が既知信号で正しい.
- scoreが欠損metricに対して安全に動く.

---

### Module 13: Sweep and Optimization Engine

#### 目的

大量条件探索, 並列実行, 結果集計, 上位候補抽出を行う.

#### 実装物

- `include/f700f/sweep/sweep_config.hpp`.
- `include/f700f/sweep/sweep_runner.hpp`.
- `src/sweep/*.cpp`.
- `src/apps/f700f-sweep.cpp`.
- `python/f700f_lab/sweep.py`.
- `python/f700f_lab/analyze.py`.
- `tests/integration/test_sweep_small.cpp`.
- `docs/sweep.md`.

#### 要件

- grid search.
- random search.
- Latin hypercube optional.
- runごとのseed分配.
- 並列実行.
- 中断再開.
- 結果JSONL集約.
- top-k候補抽出.

#### Acceptance criteria

- 小規模sweepが完走する.
- 中断後にresumeできる.
- top-k summary CSVを出力できる.

---

### Module 14: AI Integration Layer

#### 目的

将来のAI音声強調, neural codec, neural estimatorを接続する基盤を作る.

#### 実装物

- `include/f700f/ai/onnx_runtime_wrapper.hpp`.
- `include/f700f/ai/audio_postprocess_onnx.hpp`.
- `src/ai/*.cpp`.
- `python/f700f_lab/train_ai.py`.
- `python/f700f_lab/export_onnx.py`.
- `tests/unit/test_ai_identity.cpp`.
- `docs/ai_integration.md`.

#### 要件

- ONNX Runtimeはoptional dependencyにする.
- identity modelまたはmock modelでテスト可能にする.
- AIなしで全テストが通る.
- AI postprocessorはAudioBlockを入力しAudioBlockを返す.
- neural codecは将来拡張としてinterfaceだけ定義してよい.

#### Acceptance criteria

- ONNX無効buildが通る.
- ONNX有効buildでidentity model推論が通る.
- postprocess前後のWAVを比較できる.

---

### Module 15: Reporting and Visualization

#### 目的

実験結果を人間が判断しやすいreportにまとめる.

#### 実装物

- `python/f700f_lab/plot.py`.
- `python/f700f_lab/report.py`.
- `reports/templates/summary.md.j2`.
- `tools/package_report.sh`.
- `docs/reporting.md`.

#### 要件

- SNR vs score.
- SNR vs WER.
- fading severity vs score.
- mode別boxplot.
- Pareto frontier.
- 音声サンプル一覧HTMLまたはMarkdown.
- ABX主観評価用package.

#### Acceptance criteria

- sample JSONLからsummary reportを生成できる.
- plot PNGを出力できる.
- top候補の音声ファイルを並べた比較manifestを出せる.

---

### Module 16: Golden Tests, Benchmark, Integration QA

#### 目的

全体品質を守るgolden test, performance benchmark, regression管理を行う.

#### 実装物

- `tests/golden/README.md`.
- `tests/golden/generate_golden.cpp`.
- `tests/integration/test_end_to_end_regression.cpp`.
- `src/apps/f700f-bench.cpp`.
- `docs/qa.md`.

#### 要件

- deterministic outputs for fixed seeds.
- small golden vectorsをrepoに含める.
- 大きなgolden audioは外部downloadまたは生成scriptにする.
- benchmarkはreal-time factorを出す.
- CIでは短時間テスト, localではfull test.

#### Acceptance criteria

- fixed seedでmetricが安定.
- 性能退行を検出できる.
- full integration testで700F candidate, 700E baseline, SSB baselineが最低1条件ずつ通る.

---

## 14. Codexオーケストレーション方針

### 14.1 最初のCodexが行うこと

最初にこの文書を受け取るCodexは, 次を行う.

1. `develop` branchを作成し, 以後のbase branchにする. `master` へ直接commitしない.
2. `issues/ISSUE-0001-bootstrap-repository.md` を作成し, Statusを `in_progress` にしてから作業する.
3. ISSUE-0001用branchを `develop` から作る. 例: `chore/project/ISSUE-0001-bootstrap-repository`.
4. `docs/codex_task_log.md` を作成する.
5. `VERSION`, `versions.toml`, `CHANGELOG.md` を作成し, project versionを `v0.0.1` にする.
6. `docs/README.md`, `docs/versioning.md`, `docs/milestones.md`, `docs/tdd_policy.md`, `docs/ci.md`, `docs/git_workflow.md`, `docs/submodule_policy.md`, `issues/README.md`, `reports/README.md` を作成する.
7. 16モジュールすべてに `VERSION`, `CHANGELOG.md`, `docs/`, `issues/`, `reports/` と各 `README.md` を作成し, module versionを `v0.0.1` にする.
8. `.github/workflows/ci.yml`, `.github/workflows/governance.yml`, `.github/workflows/nightly-sweep.yml`, `.github/workflows/security.yml` の初期版を作成する.
9. `tools/check_governance.py`, `tools/check_versions.py`, `tools/check_submodules.py`, `tools/run_ci_local.sh` の最小版を作成する.
10. Codec2/FreeDVを `external/codec2` submoduleにするか, 一旦placeholderにするかを `docs/submodule_policy.md` と `docs/codec2_verification_notes.md` に記録する.
11. 16モジュールの実装順序をissueとして整理する. 全体issueは `issues/`, module issueは各 `modules/NN_<name>/issues/` に置く.
12. Module 1からModule 3までを先にTDD優先で実装する.
13. Module 4以降は, 各モジュール用の作業指示mdを `docs/tasks/module_##.md` として生成する.
14. 各module taskには, 入出力interface, 依存module, テスト, acceptance criteria, 対応issue, branch名, target version/milestoneを明記する.
15. 下流Codexは自分のmoduleだけでなく, 境界protocolに違反していないかを確認する.

### 14.2 下流Codex向け共通指示テンプレート

各module task mdには次を含める.

```markdown
# Module ##: <name>

## 対応issue

- modules/NN_<name>/issues/ISSUE-0000-<slug>.md

## Branch

- Base branch: develop
- Work branch: feature/mNN/ISSUE-0000-<slug>
- Worktree: ../f700f-wt-mNN-issue-0000
- Merge target: develop

## Target version and milestone

- Project version:
- Module version:
- Compatibility line:
- Milestone:

## 作業開始前

- 対応issueのStatusを `in_progress` にする.
- `modules/NN_<name>/issues/README.md` を更新する.
- 指定branch/worktree上で作業していることを確認する.
- 作業計画をissueのWork logへ追記する.
- 可能な場合, 失敗するtestまたはgolden expectationを先に追加し, issueのTDD recordへ記録する.

## 作業範囲

<このmoduleで実装する範囲>

## 触ってよいファイル

- modules/NN_<name>/include/...
- modules/NN_<name>/src/...
- modules/NN_<name>/tests/...
- modules/NN_<name>/docs/...
- modules/NN_<name>/issues/...
- modules/NN_<name>/reports/...

## 触ってはいけないファイル

- 他moduleの公開interfaceを破壊しない.
- `.gitmodules`, `external/codec2`, `patches/**` を勝手に変更しない.
- `develop` や `master` へ直接commitしない.
- 破壊が必要ならdocs/protocol.mdとmodule_contracts.mdを更新し, integration Codexに確認させる.

## 依存module

- Module X
- Module Y

## 実装要件

- ...

## テスト要件

- ...

## Acceptance criteria

- ...

## 完了時に残す記録

- 対応issueのResult, Branch, TDD record, PR/Merge statusを更新し, Statusを `review` または `closed` にする.
- modules/NN_<name>/issues/README.mdを更新する.
- version変更がある場合はmodule `VERSION`, module `CHANGELOG.md`, root `versions.toml` 更新をManagerへ依頼する.
- docs/codex_task_log.mdに実装内容, 未解決点, 既知制限を書く.
- milestone相当ならmodules/NN_<name>/reports/に時刻付きreportを作成し, modules/NN_<name>/reports/README.mdを更新する.
```

### 14.3 Codex間の統合ルール

- public header変更時は必ずdocs/protocol.mdを更新する.
- 既存testを壊した場合は, 修正するか理由をtask logへ書く.
- TODOコメントは必ずissue化し, `TODO(ISSUE-0000): ...` 形式にする.
- code/docs/test/configを変更する前に, 対応issueを作成または更新する.
- milestone達成時, baseline検証完了時, integration前QA時は, rootまたはmoduleのreportsに時刻付きreportを作成する.
- 未実装stubはruntimeで明確なerrorを返す.
- mock/stubと本実装を混同しないようdescriptorに `mock=true` capabilityを入れる.

### 14.4 Manager Codex用 root AGENTS.md テンプレート

Manager CodexはPhase 1で, ルート `AGENTS.md` を次の方針で作成する. 実際のファイルでは, commandやpathが実装状況と一致するよう更新すること.

```markdown
# AGENTS.md - F700F Manager Codex

## Role
You are the Manager Codex for the FreeDV 700F exploration repository.
Your primary responsibility is system integrity, module boundary enforcement, integration, and task delegation.

## Language and stack
- Core: C++20.
- Build: CMake.
- Config: TOML.
- Results: JSONL/CSV/WAV/CF32.
- Python is allowed only for sweep orchestration, analysis, plotting, ASR WER, and AI training/export.
- ONNX Runtime is optional and must never be required for non-AI tests.

## Source of truth
Read these files before changing architecture:
- docs/architecture.md
- docs/protocol.md
- docs/module_contracts.md
- docs/simulation_method.md
- docs/scoring_method.md
- docs/issue_workflow.md
- docs/reporting.md
- docs/versioning.md
- docs/milestones.md
- docs/tdd_policy.md
- docs/ci.md
- docs/git_workflow.md
- docs/submodule_policy.md
- docs/coordination/interface_change_requests.md
- issues/README.md
- reports/README.md

## Manager-owned paths
You may edit:
- AGENTS.md
- CMakeLists.txt
- VERSION
- versions.toml
- CHANGELOG.md
- .github/**
- .gitmodules
- external/codec2
- patches/**
- cmake/**
- docs/**
- issues/**
- reports/**
- include/f700f/**
- src/apps/**
- configs/**
- tests/integration/**
- tools/**

Do not rewrite module internals unless performing an explicit integration fix. If you do, log it in docs/coordination/manager_log.md.

## Delegation
For each module, create or update:
- modules/NN_<name>/AGENTS.md
- docs/tasks/module_NN_<name>.md
- modules/NN_<name>/issues/README.md
- modules/NN_<name>/reports/README.md
- at least one module issue before assigning implementation

A module task must define:
- public interface to implement.
- allowed files.
- forbidden files.
- dependencies.
- tests.
- acceptance criteria.
- expected artifacts.

## Version, milestone, TDD, CI
- Project and all modules start at v0.0.1.
- Keep VERSION, versions.toml, module VERSION files, and CHANGELOG.md consistent.
- Same minor Y means module compatibility; different Y is incompatible unless an adapter issue is approved.
- Prefer TDD. A task should include a failing test before production code when practical.
- GitHub CI must stay green for mergeable changes.
- Milestone completion requires a timestamped report and version/milestone update.
- All Codex work must happen on issue-specific branches based on develop.
- Do not use internal Git submodules for the 16 modules unless a migration milestone explicitly approves it.
- External Codec2/FreeDV may be pinned as a Git submodule under external/codec2.

## Interface changes
No public interface change may be merged without:
1. Updating docs/protocol.md.
2. Updating docs/module_contracts.md.
3. Recording the change in docs/coordination/interface_change_requests.md.
4. Running integration tests affected by the change.

## Baseline requirements
The simulation system must include:
- SSB standard 3kHz baseline.
- SSB narrow 1.9kHz baseline.
- FreeDV 700D Level 0 emulation.
- FreeDV 700E Level 0 emulation.
- Optional FreeDV 700D/700E Level 1 external Codec2 adapter.

## Model routing
Default routing:
- Simple implementation, file moves, stubs, TOML, docs, unit-test skeletons: `spark-implementer`.
- First fallback for compile/test failures or small debugging: `low-fixer`.
- Cross-module debugging, algorithmic mistakes, simulation correctness issues: `medium-debugger`.
- Architecture, ABI, public protocol, hard DSP/FEC/modem issues, conflicting agent outputs: `xhigh-architect`.

Escalate only with a written handoff containing failing command, logs, changed files, hypothesis, and what has already been tried.
De-escalate implementation back to Spark/low after xhigh/medium produces a plan.

## Commands
Keep these commands current:
- configure: cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
- build: cmake --build build -j
- test: ctest --test-dir build --output-on-failure
- format: <define after formatter setup>
- governance-check: python tools/check_governance.py && python tools/check_versions.py && python tools/check_submodules.py

## Completion rule
A task is not complete unless the issue ticket, TDD/test evidence, build, tests, docs, README indexes, version files when relevant, and task log are updated. If the task is a milestone, a timestamped report and milestone/version update must also be created.
```

### 14.5 Module Codex用 AGENTS.md テンプレート

Manager Codexは, 各 `modules/NN_<name>/AGENTS.md` を次のテンプレートから生成し, module固有情報を埋める.

```markdown
# AGENTS.md - Module NN: <module name>

## Role
You are the Module Codex for `<module name>`.
Implement only this module and preserve all repository-level protocols.

## Working directory
Run Codex from this directory:
`modules/NN_<name>/`

## Must read first
- ../../AGENTS.md
- ../../docs/protocol.md
- ../../docs/module_contracts.md
- ../../docs/tasks/module_NN_<name>.md
- ../../docs/issue_workflow.md
- ../../docs/reporting.md
- ../../docs/versioning.md
- ../../docs/milestones.md
- ../../docs/tdd_policy.md
- ../../docs/ci.md
- ../../docs/git_workflow.md
- ../../docs/submodule_policy.md
- ../../issues/README.md
- ./README.md
- ./VERSION
- ./issues/README.md
- ./reports/README.md

## Allowed paths
You may edit:
- modules/NN_<name>/**
- modules/NN_<name>/issues/**
- modules/NN_<name>/reports/**
- tests/unit/<module-specific tests>/** if assigned.
- docs/coordination/interface_change_requests.md only to propose an interface change.

## Forbidden paths
Do not edit:
- Other modules.
- Root public headers under ../../include/f700f/** unless the Manager task explicitly allows it.
- Root CMakeLists.txt unless assigned by Manager.
- .gitmodules, external/codec2, patches/** unless explicitly assigned by Manager.
- Integration tests unless assigned by Manager.

## Branch rule
Before editing code, docs, tests, or configs, create or use the branch assigned in the issue.
The base branch must be develop.
Do not commit directly to develop or master.
Record Branch, Base branch, Worktree, and Owner Codex in the issue.

## Issue-first rule
Before editing code, docs, tests, or configs, create or update the assigned issue under ./issues/ and set it to in_progress.
Record your brief plan in the issue Work log.

## Boundary rules
- Implement the public interface exactly as described in docs/protocol.md and docs/module_contracts.md.
- Do not introduce hidden dependencies on sibling modules.
- If you need a new field or method, write an RFC in docs/coordination/interface_change_requests.md and stop until Manager approval.

## Testing
Prefer TDD: write or update a failing unit/integration/golden test before production implementation when practical.
You must add unit tests for every non-trivial behavior.
Use deterministic seeds for stochastic tests.
If TDD is not practical, record `TDD exception` and add characterization/regression tests before closing the issue.

## Escalation
If you are blocked, do not keep guessing. Create an escalation handoff with:
- failing command.
- exact error/log excerpt.
- files changed.
- what you tried.
- suspected root cause.
- requested next tier.

Use this routing:
- Spark -> low after 2 failed attempts or repeated compile/test failure.
- low -> medium for cross-module or algorithmic/debugging uncertainty.
- medium -> xhigh for public API, ABI, numerical correctness, simulation validity, or unresolved architectural tradeoff.

## Logging
At completion:
- update the assigned issue Result, Branch, PR/Merge status, and Status.
- update ./issues/README.md.
- append to ../../docs/codex_task_log.md.
- append to ./docs/module_log.md.
- create a timestamped report under ./reports/ if this is a module milestone, audit, security review, or benchmark.
- update ./reports/README.md if a report was created.

Include:
- files changed.
- tests run.
- limitations.
- assumptions.
- interface changes requested.
```

### 14.6 下流Codexへの分担順序

Manager Codexは, 依存関係の強いmoduleを先に実装させる.

1. Module 1, 2, 3を先行実装し, build/protocol/plugin基盤を固定する.
2. Module 4, 8, 12を次に実装し, 音声IO, channel, metricsを単体で動かす.
3. Module 5, 6, 7を並行実装し, codec/FEC/modemのmockとbaselineを作る.
4. Module 10を実装し, SSB/700D/700E基準系を動かす.
5. Module 11, 13, 15を実装し, end-to-end sweep/reportを完成させる.
6. Module 14はoptionalとして後続に回す.
7. Module 16でgolden, benchmark, regressionを固める.

### 14.7 module完了報告フォーマット

各Module Codexは, 完了時に次の形式で `docs/codex_task_log.md` へ追記する.

```markdown
## <YYYY-MM-DD> Module NN <name> completion

- Related issue:
- Base branch: develop
- Branch/worktree:
- Merge target: develop
- Files changed:
- Public interfaces touched:
- Tests added:
- Commands run:
- Results:
- Known limitations:
- Follow-up issues:
- Reports created:
- Interface RFCs:
```

### 14.8 Model routing and fallback policy

このプロジェクトでは, コスト, 速度, 品質のバランスを取るために, Codex作業をtask難度ごとにmodel tierへ割り当てる. ここでの `gpt-5.5-low`, `gpt-5.5-medium`, `gpt-5.5-xhigh` は, 原則として **model=`gpt-5.5` + `model_reasoning_effort` の別名** として扱う. 環境で同名aliasを使える場合はaliasを使ってよい. 使えない場合は `.codex/agents/*.toml` で明示設定する.

#### 14.8.1 Model tier定義

| Tier | Codex custom agent | Model設定 | 使う場面 |
|---|---|---|---|
| T0 | `spark-implementer` | `gpt-5.3-codex-spark` | 簡単な実装, stub, docs, CMake小修正, TOML, unit test骨格, 機械的refactor |
| T1 | `low-fixer` | `gpt-5.5`, `model_reasoning_effort="low"` | Sparkで失敗した小さなcompile/test修正, 単一module内の軽いdebug |
| T2 | `medium-debugger` | `gpt-5.5`, `model_reasoning_effort="medium"` | 複数moduleの整合性, algorithmic bug, integration failure, channel/metric妥当性確認 |
| T3 | `xhigh-architect` | `gpt-5.5`, `model_reasoning_effort="xhigh"` | ABI/protocol変更, hard DSP/FEC/modem問題, 700D/700E reference不一致, simulation設計の根本判断 |
| R | `reviewer` | `gpt-5.5`, `model_reasoning_effort="medium"` or `high` | merge前review, acceptance criteria確認, task完了判定 |

`xhigh` が環境で利用できない場合は `high` へ自動fallbackする. `gpt-5.3-codex-spark` が利用できない場合は, 高速/低コストの利用可能modelへaliasを向け替える. 実際のmodel名は必ず `docs/coordination/model_routing.toml` で一元管理し, AGENTS.md内に分散してhardcodeしない.

#### 14.8.2 Task分類

Manager Codexはtask作成時に次を設定する.

```toml
[task_model_routing]
initial_agent = "spark-implementer"
max_agent = "medium-debugger"
escalation_after_failed_attempts = 2
requires_reviewer = true
requires_manager_approval = false
```

目安は次とする.

| Task種類 | 初期担当 | 最大fallback |
|---|---|---|
| README, docs, TOML, CMake target追加, directory作成 | Spark | low |
| simple C++ class, mock/stub, getter/setter, serialization, CLI option | Spark | low |
| deterministic unit test, golden file generator, manifest出力 | Spark | medium |
| compile error, linker error, CI setup failure | Spark -> low | medium |
| FEC/OFDM/channelの軽いbug | low | medium |
| public interface変更, plugin ABI変更, `ModeDescriptor`変更 | medium | xhigh |
| SSB/700D/700E emulationの妥当性, SNR定義, metric設計 | medium | xhigh |
| 700F探索結果の解釈, Pareto frontier採用判断 | medium | xhigh |
| 複数Codexの出力衝突, merge conflict方針 | manager + medium | xhigh |

#### 14.8.3 Escalation trigger

Module Codexは次の条件を満たしたら, 作業を止めてfallbackする.

- 同じcompile errorを2回以上修正しても解消しない.
- 同じunit/integration testが2回以上失敗する.
- 修正範囲が自module外へ広がる.
- public header, C ABI, TOML schema, artifact schema変更が必要になる.
- 指標値やsimulation結果が直感と矛盾し, 実装bugか設計問題か判断できない.
- 700D/700E Level 0とLevel 1の差が大きく, 単純な設定差で説明できない.
- 30分以上進展がない.
- 変更量が当初taskの2倍以上に広がった.

#### 14.8.4 Escalation handoff format

fallback時は `docs/coordination/escalation_log.md` へ次を追記し, 上位tierへ渡す.

```markdown
## Escalation <YYYY-MM-DD HH:MM> Module NN

- From agent:
- Requested next agent:
- Branch/worktree:
- Task file:
- Failing command:
- Error/log excerpt:
- Files changed:
- Tests passed:
- Tests failed:
- What was tried:
- Current hypothesis:
- Questions for next agent:
- Rollback safety:
```

上位tierは, いきなり大改修せず, まず次を行う.

1. 失敗を再現する.
2. 変更差分を読む.
3. root cause候補を2つ以上書く.
4. 最小修正案を提示する.
5. 必要ならManager承認を要求する.

#### 14.8.5 De-escalation rule

T2/T3が設計方針や修正計画を決めた後, 単純実装に戻せる作業はSparkまたはlowへ戻す. T3が長時間コード実装を抱え続けない. T3は **難問の分解, 方針決定, review** を主務にする.

#### 14.8.6 `.codex/agents/` templates

Manager CodexはPhase 1で次のcustom agent定義を作る. 実際のCodex設定形式が変わった場合は, 公式docsに合わせて更新する.

` .codex/agents/spark-implementer.toml `:

```toml
name = "spark-implementer"
description = "Fast implementation worker for simple, well-scoped code changes."
model = "gpt-5.3-codex-spark"
model_reasoning_effort = "low"
developer_instructions = "Implement only the assigned module task. Prefer small diffs. Stop and escalate after two failed attempts. Always run the requested unit tests."
```

` .codex/agents/low-fixer.toml `:

```toml
name = "low-fixer"
description = "Low-effort gpt-5.5 fixer for compile errors and small debugging tasks."
model = "gpt-5.5"
model_reasoning_effort = "low"
developer_instructions = "Fix focused compile, link, and unit-test failures. Do not redesign public interfaces. Escalate cross-module or algorithmic uncertainty."
```

` .codex/agents/medium-debugger.toml `:

```toml
name = "medium-debugger"
description = "Balanced gpt-5.5 debugger for cross-module and algorithmic issues."
model = "gpt-5.5"
model_reasoning_effort = "medium"
developer_instructions = "Diagnose integration failures, algorithmic bugs, and simulation inconsistencies. Prefer root-cause fixes over local patches. Request Manager approval for interface changes."
```

` .codex/agents/xhigh-architect.toml `:

```toml
name = "xhigh-architect"
description = "High-reasoning architect for hard DSP, ABI, protocol, and simulation-design issues."
model = "gpt-5.5"
model_reasoning_effort = "xhigh"
developer_instructions = "Handle architecture, public protocol, ABI, numerical correctness, and difficult design tradeoffs. Produce a plan and delegate implementation back down when possible."
```

` .codex/agents/reviewer.toml `:

```toml
name = "reviewer"
description = "Review agent for acceptance criteria, boundary violations, tests, and docs."
model = "gpt-5.5"
model_reasoning_effort = "medium"
developer_instructions = "Review diffs against module task, AGENTS.md, protocol docs, tests, and acceptance criteria. Do not implement unless explicitly asked."
```

### 14.9 Manager routing examples

#### Example 1: simple module stub

```text
Spawn `spark-implementer` in modules/04_audio_io_dataset.
Task: create wav_io interface stub, dummy tests, and docs skeleton according to docs/tasks/module_04_audio_io_dataset.md.
Do not edit other modules. Escalate after two failed build/test attempts.
```

#### Example 2: compile failure after Spark

```text
Spawn `low-fixer` in modules/04_audio_io_dataset.
Use docs/coordination/escalation_log.md entry <id>.
Reproduce the failure, fix the smallest possible issue, run the module unit tests, and update the log.
```

#### Example 3: 700D/700E emulation mismatch

```text
Spawn `medium-debugger` for modules/10_reference_baselines_ssb_700d_700e and modules/07_ofdm_modem_waveform.
Compare Level 0 and Level 1 artifacts for 700D/700E. Identify whether the mismatch is due to descriptor values, channel model, modem implementation, or external adapter assumptions.
Escalate to `xhigh-architect` if a public protocol or numerical definition change is required.
```

#### Example 4: public protocol decision

```text
Spawn `xhigh-architect` at repository root.
Task: decide whether `ModeDescriptor` should represent reference modes and 700F candidates with one schema or separate schemas. Produce an RFC and migration plan. Do not directly rewrite all modules unless Manager approves.
```

---

## 15. 実装順序

### Phase 0: 調査と検証

- Codec2/FreeDVの現在リポジトリを確認.
- 700Dおよび700Eの現在仕様を確認.
- ONNX Runtime C++導入方法を確認.
- `docs/codec2_verification_notes.md` 作成.

Exit criteria:
- 700Dおよび700E baselineに関する確認メモがある.
- SSB/700D/700E reference baseline方針が決まっている.
- 外部依存のビルド方針が決まっている.

### Phase 1: Foundation

対象: Module 1, 2, 3.

Exit criteria:
- CMake buildが通る.
- ルート `AGENTS.md` と全 `modules/*/AGENTS.md` が生成されている.
- 全 `docs/tasks/module_*.md` が生成されている.
- `.codex/agents/spark-implementer.toml`, `low-fixer.toml`, `medium-debugger.toml`, `xhigh-architect.toml`, `reviewer.toml` が生成されている.
- `docs/coordination/model_routing.toml` が生成されている.
- protocol型がある.
- registryとplugin dummyが動く.

### Phase 2: Audio, Codec, FEC, Modem minimal path

対象: Module 4, 5, 6, 7.

Exit criteria:
- dummy codec + repetition/no-FEC + simple OFDMでend-to-end bit pathが動く.
- no-channel BER=0.

### Phase 3: Channel and SSB baseline

対象: Module 8, 9, 10.

Exit criteria:
- AWGN, freq offset, multipathが動く.
- SSB baselineが音声出力できる.
- 700D/700E Level 0 emulationがpipelineへ接続できる.
- 700D/700E Level 1 external adapterが存在しない環境でskipできる.

### Phase 4: Pipeline and metrics

対象: Module 11, 12.

Exit criteria:
- `f700f-sim` で1 runを保存できる.
- metrics JSONが出る.
- SSBとdigitalを同じ形式で比較できる.

### Phase 5: Sweep and reporting

対象: Module 13, 15.

Exit criteria:
- 小規模sweepが完走.
- summary reportが出る.
- top-k候補が抽出される.

### Phase 6: 700F candidates

対象: Module 5, 6, 7, 11, 12, 13を横断.

Exit criteria:
- 700F-A/B/Cのconfigがあり, end-to-end実行できる.
- 700D baseline, 700E baseline, SSB standard, SSB narrowと比較できる.

### Phase 7: AI optional layer

対象: Module 14.

Exit criteria:
- AIなしbuild, AIありbuildが両方成立.
- identity ONNX postprocessorが動く.

### Phase 8: QA and final exploration

対象: Module 16 + all.

Exit criteria:
- deterministic golden tests.
- benchmark.
- initial full sweep report.
- 改善候補と次の実装課題がdocsにまとまっている.

---

## 16. 実験計画

### 16.1 初期実験

目的: pipelineの妥当性確認.

条件:

- Modes: dummy_digital, ssb_standard, ssb_narrow.
- Channels: no_channel, AWGN SNR 10/6/2/0 dB.
- Dataset: 5秒から10秒の短い音声3本.

成功条件:

- 出力音声が保存される.
- metricsが出る.
- SNR低下でscoreが下がる.

### 16.2 Reference baseline実験: SSB, 700D, 700E

目的: SSB, 700D, 700Eの基準傾向を確認し, 700F探索の比較軸を固定する.

条件:

- Modes: 700D Level 0, 700E Level 0, ssb_standard_3k, ssb_narrow_1p9k.
- Optional modes: 700D Level 1 external adapter, 700E Level 1 external adapter.
- Channels: AWGN, mild fading, moderate fading.
- SNR: -2, 0, 2, 4, 6, 8 dB.

成功条件:

- SNR上昇で700D/700E指標が改善.
- 700Dと700EでAWGN, fast fading, multipathの傾向差が出る.
- fading条件でsync loss等が観測される.
- SSBのアナログ劣化音声が比較出力される.

### 16.2A baseline比較の必須出力

Reference baseline実験では, 次の比較表を必ずreportへ出す.

- SSB standard 3kHz vs SSB narrow 1.9kHz.
- SSB standard 3kHz vs 700D Level 0.
- SSB standard 3kHz vs 700E Level 0.
- 700D Level 0 vs 700E Level 0.
- 700D Level 0 vs 700D Level 1, Level 1利用可能時.
- 700E Level 0 vs 700E Level 1, Level 1利用可能時.

このbaseline比較がない状態で700F探索の結論を出してはならない.

### 16.3 700F粗探索

目的: 有望な設計領域を探す.

条件:

- Modes: 700F-A/B/C + random generated 20候補.
- Channels: hf_mild, hf_moderate, dx_selective.
- SNR: -4から8 dB.
- Doppler: 0.5から4 Hz.
- Delay spread: 1から6 ms.
- Freq offset: 0から200 Hz.

成功条件:

- top-k候補が出る.
- 700Eより明瞭度が高い候補があるか確認.
- SSB standardまたはSSB narrowより平均scoreが高い条件を探す.

### 16.4 700F精密探索

目的: 上位候補の周辺を細かく探索.

条件:

- Top 5 candidates.
- Parameter refinement.
- Datasetを増やす.
- ASR WERと主観評価packageを出す.

成功条件:

- Pareto frontierが得られる.
- 700F候補の推奨profileが1から3個に絞られる.
- 強みと弱みがreport化される.

---

## 17. 依存ライブラリ候補

### 17.1 C++

| 用途 | 候補 |
|---|---|
| TOML | tomlplusplus |
| JSON | nlohmann/json |
| FFT | FFTW, pocketfft, kissfft. 最初は軽量実装優先 |
| WAV | libsndfile or dr_wav. 最初はdr_wavでも可 |
| CLI | CLI11 |
| Testing | Catch2 or doctest |
| ONNX | ONNX Runtime |
| Codec2 | official codec2 source |

### 17.2 Python

| 用途 | 候補 |
|---|---|
| Data | pandas, numpy |
| Plot | matplotlib |
| Config | tomli/tomllib, pydantic optional |
| ASR | whisper系, faster-whisper, optional |
| Audio metric | pystoi, optional |
| AI | torch, onnx, onnxruntime |

注意: Python依存はoptionalに分ける. C++ core buildにPython環境を必須化しない.

---

## 18. 品質基準

### 18.1 コード品質

- C++20の範囲で明瞭に書く.
- RAIIを徹底する.
- raw pointer所有権を避ける.
- interface境界で例外を投げるかResultを返すか統一する. C ABIではerror codeのみ.
- 大規模配列は不要コピーを避ける.
- DSP処理は後からSIMD化できるよう連続メモリにする.
- 乱数は明示的なgeneratorを渡す.

### 18.2 テスト品質

- できるだけTDDで進める. issueにTest planを書き, 可能なら失敗するtestを先に追加する.
- 各moduleにunit test.
- module間はintegration test.
- deterministic testを優先.
- stochastic channelはseed固定.
- audio metricは許容誤差を設定.
- golden vectorは小さく保つ.
- TDD例外を使った場合はissueに理由を書き, close前にcharacterization/regression/golden testを追加する.
- GitHub CIでunit testとintegration smoke testを実行する.

### 18.2.1 GitHub CI品質

- PR/pushでは軽量CIを必須にする.
- long sweepはnightlyまたはmanualに分ける.
- governance checkでissue, docs, reports, versionの整合性を確認する.
- CI failureは関連issueに記録し, 修正作業もissue-firstで行う.

### 18.2.2 Version品質

- projectとmoduleは `v0.0.1` から始める.
- `versions.toml` と各 `VERSION` はCIで検査する.
- module間互換は同じminor `Y` に限定する.
- `Z` だけの変更ではpublic protocolを破壊しない.

### 18.3 性能基準

初期性能目標:

- Release buildで1分音声の単一条件simが実時間の10倍以内で完了すること.
- sweepは並列実行でCPU core数に応じてスケールすること.
- メモリ使用量は1 runあたり数百MB未満を目標にする.

最適化は正しさの後に行う.

---

## 19. 実装上の重要な判断

### 19.1 700E互換性と700F探索の優先順位

700Eのbit-exact互換より, 探索可能な700F基盤を優先する. ただし700E baselineの実測傾向は必ず取る.

推奨:

- Codec2/FreeDV API adapterで既存700E相当を評価.
- 独自OFDM engineで700F candidateを探索.
- 可能であれば後で700E-like engineとFreeDV API baselineを比較.

### 19.2 Codec選択

最初はCodec2 adapterを使う. 700Fの音声帯域3 kHz向け最適化codecは後続課題とする.

将来:

- Codec2 1300/1600系.
- LPCNet/ニューラルcodec.
- 3 kHz明瞭度特化codec.
- 文字化/TTS系のText Voice mode.

これらを同じIAudioCodecまたは別interfaceで扱えるようにする.

### 19.3 AIの扱い

AIは評価や後処理には使うが, 初期700F modemの成立条件に含めない. AIありでだけ勝つ設計にすると, codec/modemの本質評価が困難になる.

### 19.4 SSB比較の扱い

SSBは人間の補完込みで強い. 客観指標だけでは不十分. したがって, 最終比較には必ず音声サンプルpackageと主観評価手順を含める.

---

## 20. 出力物一覧

最終的に次の成果物を作る.

| 成果物 | 内容 |
|---|---|
| `f700f-sim` | 単一条件simulation CLI |
| `f700f-sweep` | パラメータ探索CLI |
| `f700f-render` | waveform/audio出力補助CLI |
| `f700f-bench` | performance benchmark |
| `configs/modes/700f_*.toml` | 700F候補設定 |
| `configs/channels/*.toml` | channel設定 |
| `reports/initial_sweep/summary.md` | 初期探索report |
| `reports/initial_sweep/results.jsonl` | 全run metrics |
| `reports/initial_sweep/top_candidates.csv` | 上位候補 |
| `reports/initial_sweep/audio_samples/` | 聴感比較WAV |
| `VERSION` | project version. 初期値 `v0.0.1` |
| `versions.toml` | project/module/protocol/ABI version manifest |
| `CHANGELOG.md` | project変更履歴 |
| `modules/##_name/VERSION` | module version. 初期値 `v0.0.1` |
| `.github/workflows/*.yml` | GitHub CI, governance, nightly sweep, security workflow |
| `.gitmodules` | 外部上流依存をsubmoduleにする場合のmanifest |
| `patches/codec2/` | submoduleへ直接commitしないためのpatch置き場 |
| `docs/architecture.md` | 設計書 |
| `docs/issue_workflow.md` | issue-first運用仕様 |
| `docs/reporting.md` | report運用仕様 |
| `docs/versioning.md` | project/module versioning仕様 |
| `docs/milestones.md` | milestoneとversion対応表 |
| `docs/tdd_policy.md` | TDD方針 |
| `docs/ci.md` | GitHub CI運用仕様 |
| `docs/git_workflow.md` | branch/worktree/Git Flow運用仕様 |
| `docs/submodule_policy.md` | 外部submodule採用/更新/禁止事項 |
| `issues/README.md` | 全体issue index |
| `reports/README.md` | 全体report index |
| `docs/protocol.md` | 境界protocol |
| `docs/scoring_method.md` | score仕様 |
| `.codex/agents/*.toml` | Codex subagent/custom agent定義 |
| `docs/coordination/model_routing.toml` | task難度別model routing定義 |
| `docs/coordination/escalation_log.md` | fallback/escalation記録 |

---

## 21. 完了判定

プロジェクトの初期完了は次を満たすこと.

- C++20 coreがビルド可能.
- projectと全moduleが `v0.0.1` 以上のversionを持ち, `versions.toml` と整合する.
- GitHub CIの軽量workflowが存在し, local同等commandを実行できる.
- `develop` を統合branchとし, 各Codexがissue単位branch/worktreeで作業する運用がdocsに明記されている.
- 内部16moduleをsubmodule化しない方針と, Codec2/FreeDVの外部submodule採用方針がdocsに明記されている.
- 主要実装はTDDまたはTDD例外記録付きで進められている.
- 700F-A/B/Cがend-to-end simulation可能.
- SSB standard/narrow baselineと比較可能.
- AWGN, fading, multipath, frequency offset, impulse noiseが可変.
- metrics JSONLとsummary reportが出力可能.
- 小規模sweepでtop candidatesが出る.
- 各moduleがinterface越しに差し替え可能.
- AI optional layerがidentity modelで動く, またはAI無効buildで完全動作する.
- `docs/codex_task_log.md` に未解決点が整理されている.
- root/module `AGENTS.md` と `.codex/agents/*.toml` が存在し, routing policyがtask mdに反映されている.

---

## 22. 最初のCodexへの具体的初回指示

あなたは最初のオーケストレータCodexである. 次の順に作業すること.

1. この文書を `docs/codex_master_instruction.md` に保存する.
2. `develop` branchを作り, 以後の作業base branchにする. `master` へ直接commitしない.
3. `VERSION`, `versions.toml`, `CHANGELOG.md`, `docs/versioning.md`, `docs/milestones.md`, `docs/tdd_policy.md`, `docs/ci.md`, `docs/git_workflow.md`, `docs/submodule_policy.md` を作る.
4. `.github/workflows/ci.yml`, `.github/workflows/governance.yml`, `.github/workflows/nightly-sweep.yml`, `.github/workflows/security.yml` の初期版を作る.
5. `tools/check_governance.py`, `tools/check_versions.py`, `tools/check_submodules.py`, `tools/run_ci_local.sh` の最小版を作る.
6. `docs/tasks/` を作る.
7. セクション13のModule 1から16を, それぞれ `docs/tasks/module_01_repository_build_ci.md` のような個別taskに分解する.
8. 各moduleに `VERSION` と `CHANGELOG.md` を作り, 初期値を `v0.0.1` にする.
9. Codec2/FreeDVを `external/codec2` submoduleにするか, 一旦placeholderにするかを `docs/submodule_policy.md` と `docs/codec2_verification_notes.md` に記録する.
10. Module 1, 2, 3をTDD優先で実装する. 各作業はissue単位branchから始める.
11. 実装した範囲でbuildとtestを通す.
12. `docs/codex_task_log.md` に, 実装済み, 未実装, 注意点, 次に渡すべき作業を書く.
13. 下流Codexに渡す場合は, 個別task mdと関連docsだけで作業できるようにする.

このプロジェクトは探索基盤であり, 1回で完成させる必要はない. ただし, 毎段階でbuild可能, test可能, 再現可能な状態を保つこと.

---

## 23. 禁止事項

- interfaceを通さず具象実装に直接依存すること.
- seed未記録の乱数を使うこと.
- SNR定義を曖昧にしたまま結果を出すこと.
- mock実装を本実装として扱うこと.
- Python依存をC++ core buildの必須条件にすること.
- AIなしで動かない設計にすること.
- 700FがSSBを「全条件で上回る」と報告すること.
- 主観評価なしに音声品質の最終勝敗を断言すること.

---

## 24. 推奨される最初のコミット構成

```text
commit 1: repository skeleton and CMake
commit 2: core protocol types and config loader
commit 3: module registry and plugin ABI skeleton
commit 4: audio IO and test WAV generator
commit 5: dummy codec, no-FEC, repetition FEC
commit 6: simple OFDM modem no-channel test
commit 7: AWGN and frequency offset channel
commit 8: pipeline runner with dummy digital mode
commit 9: SSB baseline
commit 10: metrics and JSONL output
commit 11: sweep runner
commit 12: 700F-A/B/C candidate configs
commit 13: reporting scripts
commit 14: optional ONNX identity postprocessor
commit 15: golden tests and initial sweep report
```

---

## 25. 700F探索で最初に見るべき判断ポイント

1. 700F-AがSSB narrow 1.9 kHzより明瞭か.
2. 700F-AがSSB standard 3 kHzに対して, どのSNR/フェージング条件で勝つか.
3. 700F-Bが700Eより音声品質を上げつつフェージング耐性を維持できるか.
4. 700F-Cが2020系高音質路線ほど脆くならず, 良条件でSSBより自然に聞こえるか.
5. FECを厚くするより, UEPと短い再同期を重視した方がscoreが高いか.
6. CP 6 msと8 msの差がDX選択性フェージングで効くか.
7. carrier count 25/27/29のどこが帯域と耐性のバランスが良いか.
8. ASR WERと人間の印象が乖離する条件はどこか.

---

## 26. 最終レポートに必ず含める内容

- 700F候補の設計表.
- 比較対象の700E, SSB standard, SSB narrowの設定.
- チャンネル条件とSNR定義.
- Dataset概要.
- Metric定義.
- SNR別score plot.
- フェージング条件別score plot.
- BER/FER/sync loss表.
- ASR WER表, optional.
- 聴感比較WAVへのリンク.
- Pareto frontier.
- 上位候補の採用理由.
- 弱点と次の改善案.
- 「SSBを全条件で上回るものではない」という注意書き.

---

## 27. 補足: 将来拡張の方向

将来は次を追加できるようにする.

- 700F-GPU accelerated modem.
- AI channel estimator.
- AI post-fec decoder helper.
- Neural speech enhancement.
- Neural low bitrate codec.
- Text Voice mode: ASR -> text protocol -> TTS.
- 実機サウンドカードI/O.
- FreeDV GUIまたは外部GUI連携.
- SDR IQ入出力.
- Distributed sweep server.
- Human listening test web UI.

これらはすべてplugin boundaryを通して追加する. Core pipelineを直接肥大化させないこと.

---

## 28. 参考情報

Codexは作業開始時に必ず最新の一次情報を確認すること. 少なくとも次を確認する.

- Codec2 official repository.
- Codec2 `README_freedv.md`.
- Codec2 `src/freedv_api.c` と関連mode定義.
- Rowetelの700E開発メモ.
- ONNX Runtime official C/C++ API docs.

この文書の設計値は探索の出発点であり, 最終仕様ではない. 実験結果に基づいて700F候補を更新すること.


### 28.1 Reference baseline source notes

- Codec2/FreeDV source and `README_freedv.md` must be checked at implementation time. Initial 700D/700E values in this document are not normative.
- Initial values used here are based on publicly documented FreeDV mode tables: 700D uses Codec2 700C, 17 carrier coherent OFDM/QPSK, RF BW 1000Hz, raw 1900bit/s, LDPC(224,112), text 25bit/s, SNR min -2dB, multipath fair; 700E uses Codec2 700C, 21 carrier coherent OFDM/QPSK, RF BW 1500Hz, raw 3000bit/s, LDPC(112,56), text 25bit/s, SNR min 1dB, multipath good.
- FreeDV documentation states that multipath is a major issue for HF digital voice, and analog SSB would be rated as good. This is why SSB must remain a baseline.
- FreeDV documentation also notes that 700D is optimized for low SNR with strong FEC but can degrade on fast multipath fading due to low pilot symbol rate and modest cyclic prefix. This is why 700D and 700E must be evaluated separately rather than treated as one baseline.


### 28.2 Codex model routing notes

- Codex official docs currently list `gpt-5.5`, `gpt-5.4`, `gpt-5.4-mini`, and `gpt-5.3-codex-spark` as recommended models, with `gpt-5.3-codex-spark` described as optimized for near-instant real-time coding iteration.
- `gpt-5.5-low`, `gpt-5.5-medium`, and `gpt-5.5-xhigh` in this document are operational aliases for `model = "gpt-5.5"` plus `model_reasoning_effort = "low"`, `"medium"`, or `"xhigh"`.
- Codex subagent docs describe custom agents under `.codex/agents/` and allow custom agent configs to specify fields including `model` and `model_reasoning_effort`.
- Codex config reference lists `model_reasoning_effort` values including `minimal`, `low`, `medium`, `high`, and `xhigh`, with `xhigh` model-dependent. If `xhigh` is unavailable, fallback to `high`.
