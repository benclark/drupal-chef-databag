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
