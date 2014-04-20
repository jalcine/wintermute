# -*- mode: ruby -*-
# vi: set ft=ruby :

VAGRANTFILE_API_VERSION = "2"

Vagrant.configure(VAGRANTFILE_API_VERSION) do |config|
  config.vm.box = "quantal64"
  config.vm.box_check_update = false
  config.vm.provider "virtualbox" do |vb|
     vb.gui = false
     vb.customize ["modifyvm", :id, "--memory", "2048"]
  end
end
