# Github Workflow :: TRENCHNATOR :robot:
Ops for TRENCHNATOR
This is a description of the Github workflow that will be employed in devOps of the mobile platform (Android/iOS/Web) apps.

## Branches
There will be 2 main branches,
* `master (main)` - branch contains stable production source code or release
* `Develop` - branch Beta code undergoing devlopment with new features

However, 3 other branches support the workflow.
* `Feature` - branch for new feature development on the `Develop` branch. Merged into `Develop` after PR and review
* `Release` - merge new source from `Develop` to `master`
* `Hotfix` - performing hot or quick fixes on production source on `master`  



Image Decription of Workflow

![image](https://user-images.githubusercontent.com/47186373/127318225-67226877-24cd-4e97-af30-06b4198c416f.png)


## Team Collaboration
Important to note
- Always do 

```bash
git pull
``` 

- to keep local source/repository up to date with remote to prevent potential merge conflits because your local repository could be lagging in recent  updates made to the remote
- The main development branch (default branch) is set to develop not master.
- When working on a new feature, do this on the develop branch. Create a new feature branch off the `develop` branch to implement new feature. Do
```bash
git checkout -b new-feature-branch-name
```
## when doing commit
- Commit message should be
```bash
git commit -m "new-feature-branch-name resolve #issue-number"
```
eg. 
```bash
git commit -m "login resolve #3"
```
#3 is the issue number referencing the login issue from issues 

## when doing push
- Do not merge `new feature` branch and the  `develop` branch locally. Push feature branch and create PR for review. After which new feature branch can be merged into `develop`.Do
```bash
git push origin new-feature-branch-name
```
then create PR for reveiw. After review, it wil be merged with `develop`

merged with develop

## when raising issues
- Always assign the project
- ![image](https://user-images.githubusercontent.com/47186373/127874323-2cb0270d-707c-4b45-9b5d-7444f0bbe05a.png)


Kindly enable watch for the repository
![image](https://user-images.githubusercontent.com/47186373/127831522-6ca9b5c9-2a00-4cce-822e-275118ff1682.png)

