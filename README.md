drupal-chef-databag
===================

Drupal module that creates entities (and entity bundles) corresponding to Chef server data bags.

### Requirements ###

* [Drupal](http://drupal.org) 7.x
* [Entity API](https://drupal.org/project/entity)
* [Chef Knife](https://github.com/benclark/drupal-chef-knife)

### How to use ###

1. Connect the Chef Knife module to your Chef server instance.
2. Create chef_databag entities. These are fieldable, so feel free to create any number of fields to describe your data.
3. Create chef_databag_item entities.

## Encrypted data bags ##

### Requirements ###

* Chef Databag
* [Chef Shared Secret](https://github.com/benclark/drupal-chef-shared-secret)
* [Entity reference](https://drupal.org/project/entityreference)

### How to use ###

1. Enable Chef Shared Secret, and add one or more shared secret entities.
2. Enable this module, and navigate to `admin/structure/chef_databag`.
3. Edit a data bag, then check the box next to "Encrypted data bag" and click Save. This will add the required entityreference field to the data bag. Note that this cannot be reversed, and the entityreference field is now a required field for all items (entities) in this data bag.
4. Edit an item in this data bag, and click Save. This will send the encrypted form of the JSON to Chef server.
