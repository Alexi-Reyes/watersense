<?php

namespace App\Entity;

use ApiPlatform\Metadata\ApiResource;
use App\Repository\UserRepository;
use Doctrine\Common\Collections\ArrayCollection;
use Doctrine\Common\Collections\Collection;
use Doctrine\ORM\Mapping as ORM;

#[ORM\Entity(repositoryClass: UserRepository::class)]
#[ApiResource]
class User
{
    #[ORM\Id]
    #[ORM\GeneratedValue]
    #[ORM\Column]
    private ?int $id = null;

    #[ORM\Column(length: 255)]
    private ?string $email = null;

    /**
     * @var Collection<int, WaterSense>
     */
    #[ORM\OneToMany(targetEntity: WaterSense::class, mappedBy: 'User')]
    private Collection $waterSenses;

    public function __construct()
    {
        $this->waterSenses = new ArrayCollection();
    }

    public function getId(): ?int
    {
        return $this->id;
    }

    public function getEmail(): ?string
    {
        return $this->email;
    }

    public function setEmail(string $email): static
    {
        $this->email = $email;

        return $this;
    }

    /**
     * @return Collection<int, WaterSense>
     */
    public function getWaterSenses(): Collection
    {
        return $this->waterSenses;
    }

    public function addWaterSense(WaterSense $waterSense): static
    {
        if (!$this->waterSenses->contains($waterSense)) {
            $this->waterSenses->add($waterSense);
            $waterSense->setUser($this);
        }

        return $this;
    }

    public function removeWaterSense(WaterSense $waterSense): static
    {
        if ($this->waterSenses->removeElement($waterSense)) {
            // set the owning side to null (unless already changed)
            if ($waterSense->getUser() === $this) {
                $waterSense->setUser(null);
            }
        }

        return $this;
    }
}
