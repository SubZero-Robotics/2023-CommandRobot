# Team 5690 SubZero Robotics 2023 Command Robot

## Table of contents

- [Team 5690 SubZero Robotics 2023 Command Robot](#team-5690-subzero-robotics-2023-command-robot)
  - [Table of contents](#table-of-contents)
  - [About](#about)
  - [Development Cycle](#development-cycle)
    - [Main](#main)
    - [Competition](#competition)
  - [Subsystems](#subsystems)
  - [CAN IDs](#can-ids)
  - [Network Map](#network-map)
  - [Getting started](#getting-started)
    - [Prerequisites](#prerequisites)
  - [Making changes](#making-changes)
    - [Cloning the repo](#cloning-the-repo)
    - [Issues](#issues)
    - [Branches](#branches)
    - [Adding commits](#adding-commits)
    - [Pushing commits](#pushing-commits)
    - [Pulling branches](#pulling-branches)
    - [Pull requests](#pull-requests)
  - [Necessary Hardware](#necessary-hardware)
  - [Network Map](#network-map-1)
  - [Contact](#contact)

## About

:warning: **TODO: Add about section**

## Development Cycle

### Main

Main should **always** contain known, tested, and working code that has been throroughly verified by running it on the robot. Use other development branches to create new features and test them before pulling into `main` via a Pull Request (PR). `main` cannot have code pushed to it directly, meaning a PR is **mandatory**.

Use the `main` branch for non-competition usage and practice.

### Competition

Before a competition, create a new branch based on `main` and lock it via a GitHub protection rule to prevent pushes. Only use this branch during a competition to avoid unnecessary or breaking deployments to the RIO.

:warning: ***Changes should only be made to this branch if there is a blocking error that must be fixed immediately!*** :warning:

## Subsystems

Following the WPILib command based structure we have broken our robot up into a number of subsystems. They are listed below:

| Subsystem                                                             | Purpose                                                  |
| :-------------------------------------------------------------------- | :------------------------------------------------------- |
| [Brake](src/main/include/subsystems/BrakeSubsystem.h)                 | Set or unset brakes (supports brake mode and pneumatics) |
| [Drive](src/main/include/subsystems/DriveSubsystem.h)                 | Drives robot                                             |
| [Extension](src/main/include/subsystems/ExtensionSubsystem.h)         | Extends the arm                                          |
| [Intake](src/main/include/subsystems/IntakeSubsystem.h)               | Activates the intake                                     |
| [LEDController](src/main/include/subsystems/LEDControllerSubsystem.h) | Activates the LEDs (Does yellow and purple)              |
| [RotateArm](src/main/include/subsystems/RotateArmSubsystem.h)         | Rotates the arm                                          |
| [Wrist](src/main/include/subsystems/WristSubsystem.h)                 | Rotates the wrist                                        |

## CAN IDs

|     Purpose/Name     | CAN ID | Motor/Driver Type |
| :------------------: | :----: | :---------------: |
|   Left drive lead    |   6    |      TalonFX      |
|  Left drive follow   |   5    |      TalonFX      |
|   Right drive lead   |   8    |      TalonFX      |
|  Right drive follow  |   7    |      TalonFX      |
|  Lead arm rotation   |   1    |     SparkMax      |
| Follow arm rotation  |   3    |     SparkMax      |
|    Wrist rotation    |   15   |     SparkMax      |
|    Intake spinner    |   17   |     SparkMax      |
|   Extension motor    |   50   |     SparkMax      |
| Pneumatic controller |   9    |       RevPH       |


## Network Map

|  Device   |             Address              |
| :-------: | :------------------------------: |
|  Gateway  | 10.56.90.129 (subject to change) |
|    RIO    |            10.56.90.2            |
|   Coral   |            10.56.90.4            |
| LimeLight |           10.56.90.11            |
|  Laptop   |             Dynamic              |

## Getting started

### Prerequisites

Install the following:

- [WPILib](https://docs.wpilib.org/en/stable/docs/getting-started/getting-started-frc-control-system/wpilib-setup.html)
- [Git](https://git-scm.com/download/win) (You don't need GitHub desktop)

## Making changes

### Cloning the repo

1. Install the [GitHub VS Code extension](vscode:extension/GitHub.vscode-pull-request-github)
2. Go to the GitHub tab and sign in using the account that has access to this repository
3. Click `Clone Repository` and search for `SubZero-Robotics/2023-CommandRobot`
4. Clone it into a known folder that has **no spaces** in its path
5. Open the repository in VS Code

### Issues

[GitHub issues](https://github.com/SubZero-Robotics/2023-CommandRobot/issues) are how we track which tasks need work along with who should be working on them. Pay attention to the issue number since this is how each one is uniquely identified. To create a new issue, visit the [issues tab in the repository](https://github.com/SubZero-Robotics/2023-CommandRobot/issues) and click `New issue`. Give it a descriptive title and enough information in the comment area so that anyone working on the issue knows exactly what needs to be changed/fixed. Additionally, assignees (who is working on it) and labels (what type of issue is this) can be assigned on the right.

### Branches

Branches are simply named pointers that point to a commit. Our main branch is called `main`, but we don't make changes to it directly. Instead, a new branch should be created before any code is modified. Click on the current branch in the bottom-left corner in VS Code and then click `+ Create new branch...`. The following naming conventions should be followed:

For a feature branch (major change):
`feature-<issue number>-<a few words describing the change>`

For a smaller, individual/small group branch:
`<first name>-<issue number>-<a few words describing the change>`

### Adding commits

After finishing a small change, such as modifying a method or adding a new file, a new commit should be made immediately. Go to the `Source Control` tab on the left side of VS Code and add the changed/added/deleted files from the `Changes` dropdown to `Staging` by clicking the `+`. Once the changes are staged, Type a **descriptive** commit message and then click the `Commit` button.

### Pushing commits

Commiting only applies the new commit locally. So to make it available to others in the repo, it needs to be pushed to the `remote` (GitHub's servers in this case). Either click `publish` in the `Source Control` tab if the branch hasn't been pushed before or `push` it otherwise to send the new commit to the remote's branch (this will be called `origin/<your branch's name>`).

### Pulling branches

If someone else has made their branch available on the remote, you might be wondering how other people can get those same commits into their local repo. This process is called `pulling`, and it involves downloading the remote's commits and merging them with the local branch's commits.

Sometimes, you might see a `merge conflict` appear which can happen if more than one person has worked on the same line in a file, thus making git unable to automatically merge them. To fix this, click on the conflicted file (denoted by a `!`) that opens the merge conflict editor. Once at the line(s) in question, either accept the incoming (one that exists on GitHub), the HEAD (the one that you have locally), or attempt to manually merge the two together by directly editing the line(s). Repeat this process for each line/file until all conflicts are resolved. Once done, stage the now-fixed files, create a new commit, and push.

### Pull requests

Now that all of your changes have been made (and *tested!*), it's time to get them merged into the main branch. Either click the `Create Pull Request` button in the `Source Control` tab (inline with the `SOURCE CONTROL` text, fourth one from the left) or go to the [`Pull requests` tab on GitHub](https://github.com/SubZero-Robotics/2023-CommandRobot/pulls), click `New pull request`, and set the `compare` branch to your branch. Fill in the template with a good title, a detailed description, a list of changes made, and the issue number before creating it. Assign a reviewer(s) on the right and label the PR accordingly.

The reviewer is responsible for looking over the PR, testing the changes themselves, and adding comments to the code changes if necessary. There are three possible actions a reviewer can take:

- Approve (PR is good; required to merge and should be done by a mentor or team lead)
- Disapprove (PR needs changes before it can be merged; please address the comments, click `Resolve` under each comment once fixed, then re-request a review)
- Comment (simply add a comment(s) without approving or disapproving)

## Necessary Hardware

1 Prototype 4 v2
2 Prototype 4

## Network Map

|  Device   |   Address   |
| :-------: | :---------: |
|  Gateway  | 10.56.90.1  |
|    RIO    | 10.56.90.2  |
|   Coral   | 10.56.90.4  |
| LimeLight | 10.56.90.11 |

## Contact

If you would like to contact the subzero robotics programmers, you can do so at [5690programmers@gmail.com](mailto:5690Programmers@gmail.com)

To contact the team directly, you can find us on [facebook](https://www.facebook.com/Esko-SubZero-Robotics-Team-5690-695407257248414/) or email us at [subzerorobotics@esko.k12.mn.us](mailto:subzerorobotics@esko.k12.mn.us)

Project Link: [https://github.com/subzero-robotics/2023-commandRobot](https://github.com/subzero-robotics/2023-commandRobot)
