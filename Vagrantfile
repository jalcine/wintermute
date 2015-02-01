# vi: set ft=ruby :

VAGRANTFILE_API_VERSION = "2"

Vagrant.configure(VAGRANTFILE_API_VERSION) do |config|
  config.vm.box = 'chef/ubuntu-14.04'
  config.vm.box_check_update = true
  config.ssh.forward_agent = true

  config.vm.provider 'virtualbox' do |vb|
    vb.gui = false
    vb.customize ['modifyvm', :id, '--memory', '1024']
    vb.customize ["modifyvm", :id, "--natdnshostresolver1", "on"]
    vb.customize ["modifyvm", :id, "--natdnsproxy1", "on"]
  end

  config.vm.provision 'shell' do | s |
    s.path = "./test/bootstrap"
    s.args = "--before"
  end

  config.vm.define "target" do | t |
    t.vm.box = 'chef/ubuntu-13.04'
  end

  config.vm.define "travis" do | t |
    t.vm.box = 'chef/ubuntu-12.04'
  end
end
