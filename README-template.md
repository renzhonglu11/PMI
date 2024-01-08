# Software template for PMI's Nucleo projects

This is the base template to create new software. It mainly contains the clock
initialization procedure and features to communicate via uart using nucleo's
uart-usb bridge.

## Clone repository
If you use git to track changes in your project (which you should!), the following workflow is recommended:

1. (Optional) Create remote repository (e.g. on [GitLab UPB](https://git.uni-paderborn.de/))
2. Clone or create local repository
   * When using a remote repository as per step 1, create a local clone: `git clone YOUR_REPO`

   * Otherwise, create a new repository: `git init`

3. Add the template as an alternate remote: `git remote add template git@git.uni-paderborn.de:pmi/pmi-sw-template.git`

4. Pull the template: `git pull template main`

Now, you can start with a fresh template with the following behavior:
* Adding commits will add them to your own master.
* Pushing your changes with `git push` will push them to your own remote (not the template).
* Calling `git pull template master` again will update the template.
* Updating the template will *not* overwrite your local changes.
* As long as you do not change core files, no merge conflicts should occur.

(Thanks, Manuel!)
